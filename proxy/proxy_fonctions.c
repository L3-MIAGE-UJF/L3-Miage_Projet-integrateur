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

void premier_appel_app_ext(int csock, int sock_cacheujf, char * s_id_post_inf_comp) {
	int file_temp_app_ext, file_output_app_ext;
	int pid_execution_app_ext;
	int status_pid_app_ext;

	char buffer_in[TAILLE_BUFFER], buffer_out[TAILLE_BUFFER];
	int size_in, size_out;
	
	switch(pid_execution_app_ext=fork()) {
		case 0 :
			file_temp_app_ext = open(TEMP_APP_EXT, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

			dup2(file_temp_app_ext, STDOUT_FILENO);

			close(file_temp_app_ext);

			execl(APP_EXT_TEST_PARSER,"testParsers", "1", s_id_post_inf_comp, "../infirmiere/Serveur/XML_Process/data/",(char *)0);
			exit(EXIT_FAILURE); // En cas d'echec d'execl
		break;

		default :
//sleep(2);
			printf("en attente mort execution app cpp\n");

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

			memset(buffer_out, 0, TAILLE_BUFFER);
			strcpy(buffer_out, "GET ");
			strncpy(buffer_out+strlen(buffer_out), buffer_in, size_in);
			
			//printf("\nbuffer envoyé a google :\n%s", buffer_out);

			// Envoi de la requete a google map
			if(send(sock_cacheujf, buffer_out, strlen(buffer_out), 0) < 0) {
				perror("Erreur avec la procedure send() sock_cacheujf");
				exit(errno);
			}
			char gros_tampon[3310720];
			//reception
			if((size_out=recv(sock_cacheujf, gros_tampon, 3310719,0))==-1) {
				perror("Erreur avec la procedure recv() sock_cacheujf");
				exit(errno);
			}
//printf("recu par google : %s", gros_tampon);
			if((file_output_app_ext = open(OUTPUT_APP_EXT, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP))==-1) {
				perror("Erreur lors de l'ouverture du fichier OUTPUT_APP_EXT");
				exit(EXIT_FAILURE);
			}

//fflush(stdout);
			char * shift;
			shift=strstr(gros_tampon,"<?xml");

			int lol = (int)strcspn(gros_tampon,"<?");

			if(write(file_output_app_ext, shift, size_out-lol)==-1) {
				perror("Erreur avec la procedure write()");
				exit(EXIT_FAILURE);
			}

			close(file_output_app_ext);

			second_appel_app_ext(csock, gros_tampon);

		break;
	}
}

void second_appel_app_ext(int csock, char gros_tampon[3310720]) {
	int file_output_app_ext_html;
	
	int pid_execution_app_ext;
	int status_pid_app_ext;

	char *s_id_post_inf_comp=NULL;

	int size_in;

	switch(pid_execution_app_ext=fork()) {
		case 0 :
			execl(APP_EXT_TEST_PARSER,"testParsers", "2", s_id_post_inf_comp, "../infirmiere/Serveur/XML_Process/data/",(char *)0);
			exit(EXIT_FAILURE); // En cas d'echec d'execl
		break;

		default :
			printf("en attente mort2 execution app cpp\n");

			waitpid(pid_execution_app_ext, &status_pid_app_ext, 0);

			printf("le fils est mort2, vive le fils !");

			if((file_output_app_ext_html = open(OUTPUT_APP_EXT_HTML, O_RDWR))==-1) {
				perror("Erreur avec la procedure open() file_output_app_ext_html");
				exit(EXIT_FAILURE);
			}

			if((size_in=read(file_output_app_ext_html, gros_tampon, 3310720))==-1) {
				perror("Erreur avec la procedure read() file_output_app_ext_html");
				exit(EXIT_FAILURE);
			}

			if(send(csock, gros_tampon, size_in, 0) < 0) {
				perror("Erreur avec la procedure send() csock");
				exit(errno);
			}

			//printf("\n lu dans fichier : %s ", buffer_in);
		break;
	}
}
