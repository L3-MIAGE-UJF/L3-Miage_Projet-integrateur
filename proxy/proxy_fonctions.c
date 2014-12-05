#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "libcomm/communicate.h"

#include "proxy.h"
#include "proxy_fonctions.h"

int recup_param_port(int argc, char * argv[]) {
	int port;
	int indice;

	/*
	 * On parcours argv afin d'identifier le parametre p
	 */

	if (argc>1) {
		for(indice=1;indice<argc;indice=indice+2) {
			if (argv[indice][0]=='-'&&argv[indice][2]=='\0') {
				switch (argv[indice][1]) {
					case 'p' :
						port=atoi(argv[indice+1])%65535;
					break;
					
					default:
					break;	
				}
			}
		}
	}
	return port;
}

void premier_appel_app_ext(int csock, int sock_cacheujf, char * s_id_post_inf_comp) {
	int file_temp_app_ext, file_output_app_ext;
	int pid_execution_app_ext;
	int status_pid_app_ext;

	char buffer_in[TAILLE_BUFFER], buffer_out[TAILLE_BUFFER];
	int size_in, size_out;
	
	switch(pid_execution_app_ext=fork()) {
		case 0 :

			/*
			 * Nous sommes dans le fils
			 *
			 * Ouverture d'un fichier qui servira pour enregistrer la sortie standard de l'application
			 * modification du stdout du fils par le fichier ouvert.
			 * execution de l'application tiers.
			 *
			 */

			printf("\nPremier appel app externe\n\n");					

			file_temp_app_ext = open(TEMP_APP_EXT, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

			dup2(file_temp_app_ext, STDOUT_FILENO);

			close(file_temp_app_ext);

			execl(APP_EXT_TEST_PARSER,"testParsers", "1", s_id_post_inf_comp, CHEMIN_XML_PROCESS_DATA,(char *)0);

			exit(EXIT_FAILURE); // En cas d'echec d'execl on termine le fils
		break;

		default :

printf("en attente mort execution app cpp\n");

			/*
			 * On attend la mort du fils pour aller lire ce qu'il a produit dans le fichier de sortie.
			 */

			waitpid(pid_execution_app_ext, &status_pid_app_ext, 0);

printf("le fils est mort, vive le fils !");

			if((file_temp_app_ext = open(TEMP_APP_EXT, O_RDWR))==-1) {
				perror("Erreur lors de l'ouverture de TEMP_APP_EXT");
				exit(EXIT_FAILURE);
			}

			if((size_in=read(file_temp_app_ext, buffer_in, TAILLE_BUFFER))==-1) {
				perror("Erreur avec la procedure read() file_temp_app_ext");
				exit(EXIT_FAILURE);
			}

//printf("\n lu dans fichier : %s ", buffer_in);

			/*
			 * On prépare une requète pour google map.
			 * Dans un buffer on écrit GET suivi de la requete lue dans le fichier
			 * La requète n'est pas complètement conforme à une requète http standard
			 * car il manque quelques informations, mais pour ce prototype cela ne gène pas.
			 */

			memset(buffer_out, 0, TAILLE_BUFFER);
			strcpy(buffer_out, "GET ");
			strncpy(buffer_out+strlen(buffer_out), buffer_in, size_in);
			
//printf("\nbuffer envoyé a google :\n%s", buffer_out);

			/*
			 * On envoie cette requète a google map a travers le cache UJF
			 */

			if(send(sock_cacheujf, buffer_out, strlen(buffer_out), 0) < 0) {
				perror("Erreur avec la procedure send() sock_cacheujf");
				exit(errno);
			}

			/*
			 * Partie a optimiser, on récupère les données de google pour en extraire un fichier xml.
			 * Pour l'instant il s'agit d'un gros tampon,
			 * l'objectif est d'avoir un while et des recv pour récuperer le fichier complet, nous devons utiliser select+recv
			 * car un simple read ne permet pas de pendre en compte de prendre certains cas dans le protocol http.
			 * Un do while + read permettrais de lire avec un petit tampon tout le fichier envoyé
			 * Mais beacoup de choses ne seraient pas supportés, comme les connexions persistantes.
			 * Et cela est sujet a beaucoup plus d'erreur qu'un do while + select + recv
			 */

			/*
			 * Ouverture / création du fichier qui va servir a recevoir le fichier xml venant de google
			 */

			if((file_output_app_ext = open(OUTPUT_APP_EXT, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP))==-1){
				perror("Erreur lors de l'ouverture du fichier OUTPUT_APP_EXT");
				exit(EXIT_FAILURE);
			}

			// Reception depuis google du fichier

			do {
				memset(buffer_out, 0, TAILLE_BUFFER);

				if((size_out=read(sock_cacheujf, buffer_out, TAILLE_BUFFER))==-1) {
					perror("Erreur avec la procedure read() sock_cacheujf");
					exit(errno);
				}

				if(write(file_output_app_ext, buffer_out, size_out)==-1) {
					perror("Erreur avec la procedure write()");
					exit(EXIT_FAILURE);
				}

			} while (size_out>0&&size_out<TAILLE_BUFFER);

			/*
			 * On peut fermer le fichier on a fini d'écrirer le fichier xml.
			 * Le fichier XML étant complet on peux rappeller l'application afin qu'il analyse ce xml
			 * Génère une matrice de distance, l'optimise avec l'app de RO, et génère une page html avec
			 * les résultats. Cette page html devra etre envoyée au client.
			 */

			close(file_output_app_ext);

			second_appel_app_ext(csock);

		break;
	}
}

void second_appel_app_ext(int csock) {
	int file_output_app_ext_html;
	
	int pid_execution_app_ext;
	int status_pid_app_ext;

	char *s_id_post_inf_comp=NULL;

	char buffer_in[TAILLE_BUFFER];
	int size_in;

	switch(pid_execution_app_ext=fork()) {
		case 0 :

			printf("\n Deuxieme appel app externe\n\n");					

			execl(APP_EXT_TEST_PARSER,"testParsers", "2", s_id_post_inf_comp, CHEMIN_XML_PROCESS_DATA,(char *)0);

			exit(EXIT_FAILURE); // En cas d'echec d'execl on termine le fils
		break;

		default :
printf("en attente mort2 execution app cpp\n");

			/*
			 * On attend la mort du fils pour aller lire ce la page html produite et l'afficher au client.
			 */

			waitpid(pid_execution_app_ext, &status_pid_app_ext, 0);

printf("le fils est mort2, vive le fils !");

			// On ouvre le fichier dans lequel la page html a été générée

			if((file_output_app_ext_html = open(OUTPUT_APP_EXT_HTML, O_RDWR))==-1) {
				perror("Erreur avec la procedure open() file_output_app_ext_html");
				exit(EXIT_FAILURE);
			}

			do {
				memset(buffer_in, 0, TAILLE_BUFFER);

				// On lit le fichier généré.

				if((size_in=read(file_output_app_ext_html, buffer_in, TAILLE_BUFFER))==-1) {
					perror("Erreur avec la procedure read() file_output_app_ext_html");
					exit(EXIT_FAILURE);
				}

				// On envoie le contenu lu dans le socket pour le client (Affichage de la page html)

				if(send(csock, buffer_in, size_in, 0) < 0) {
					perror("Erreur avec la procedure send() csock");
					exit(errno);
				}

			} while (size_in>0&&size_in<TAILLE_BUFFER);

			//printf("\n lu dans fichier : %s ", buffer_in);
		break;
	}
}
