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

void mort_fils() {
	int status;
	wait(&status);
}

int main(int argc, char * argv[]) {
	//Gestion de signal
	struct sigaction sig;
	sig.sa_handler=mort_fils;
	sig.sa_flags=SA_RESTART;
	sigaction(SIGCHLD, &sig, NULL);

	int port = 80;

	int pid_fils;

	int sock, csock, sock_nodejs, sock_cacheujf;

	struct sockaddr_in sin = { 0 };
	struct sockaddr_in csin = { 0 };
	struct sockaddr_in sin_nodejs = { 0 };
	struct sockaddr_in sin_cacheujf = { 0 };

	socklen_t sinsize = sizeof(csin);

	struct hostent *hostinfo_nodejs = NULL;
	const char *hostname_nodejs = "127.0.0.1";

	struct hostent *hostinfo_cacheujf = NULL;
	const char *hostname_cacheujf = "www-cache.ujf-grenoble.fr";

	char buffer_in[TAILLE_BUFFER];
	char buffer_out[TAILLE_BUFFER];

	char chaine_intercept_inf[]="/INFIRMIERE";
	char chaine_intercept_gest[] = "interface-infirmiere";

	int size_in;//, size_out;

	char *s_id_post_inf=NULL;
	char *s_id_post_inf_comp=NULL;

	/**
	 * Récupération des paramètres, numéro de port ...
	 */

	port=recup_param_port(argc, argv);
	
	/*
	 * Création de la socket d'écoute
	 */

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == INVALID_SOCKET) {
	    perror("Erreur avec la procedure socket() sock");
	    exit(errno);
	}

	/*
	 * Attachement et configuration de la socket d'écoute
	 */

	sin.sin_addr.s_addr = htonl(INADDR_ANY); // N'importe quelle adresse entrante
	sin.sin_family = AF_INET; // multiples réseaux IP - Internet
	sin.sin_port = htons(port); // Port reçu en parametre

	/*
	 * Bind du socket avec les parametres renseignés dans sin
	 */

	if(bind (sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
	    perror("Erreur avec la procedure bind() sock");
	    exit(errno);
	}

	/*
	 * Ouverture du service en écoute
	 * Le 3 permet de limiter à trois connexions simultanées
	 */

	if(listen(sock, 3) == SOCKET_ERROR) {
	    perror("Erreur avec la procedure listen() sock");
	    exit(errno);
	}

	printf("\nLancement du proxy sur le port %d\n\n", port);

	/*
	 * Debut boucle principale
	 */

	while(1) {
		/*
		 * En attente de connexion
		 * Accept bloquant, lors d'une connexion entrante on créé un socket dédié
		 */

		if((csock = accept(sock, (struct sockaddr *) &csin, &sinsize)) == INVALID_SOCKET) {
		    perror("Erreur avec la procedure accept()");
		    exit(errno);
		}

		/*
		 * Création d'un processus fils pour traiter la connexion
		 */

		switch(pid_fils=fork()) {
			case 0 :
				close(sock); // On ferme le socket d'écoute qui n'est pas utilisé dans le fils

				/*
				 * Creation des sockets pour la connexion a l'exterieur cache ujf et socket pour nodejs
				 */

				// Connexion au cache UJF

				//printf("\nProc fils, connexion a cacheujf et nodejs %d\n\n", port);

				sock_cacheujf = socket(AF_INET, SOCK_STREAM, 0);

				if(sock_cacheujf == INVALID_SOCKET) {
				    perror("Erreur avec la procedure socket()");
				    exit(errno);
				}

				hostinfo_cacheujf = gethostbyname(hostname_cacheujf);

				if (hostinfo_cacheujf == NULL) {
				    fprintf (stderr, "Unknown host %s.\n", hostname_cacheujf);
				    exit(EXIT_FAILURE);
				}

				sin_cacheujf.sin_addr = *(struct in_addr *) hostinfo_cacheujf->h_addr;
				sin_cacheujf.sin_port = htons(3128);
				sin_cacheujf.sin_family = AF_INET;

				if(connect(sock_cacheujf,(struct sockaddr *) &sin_cacheujf, sizeof(struct sockaddr)) == SOCKET_ERROR)
				{
				    perror("Erreur avec la procedure connect() cacheujf");
				    exit(errno);
				}

				// Connexion au serveur nodejs

				sock_nodejs = socket(AF_INET, SOCK_STREAM, 0);

				if(sock_nodejs == INVALID_SOCKET) {
				    perror("Erreur avec la procedure socket()");
				    exit(errno);
				}

				hostinfo_nodejs = gethostbyname(hostname_nodejs);

				if (hostinfo_nodejs == NULL) {
				    fprintf (stderr, "Unknown host %s.\n", hostname_nodejs);
				    exit(EXIT_FAILURE);
				}

				sin_nodejs.sin_addr = *(struct in_addr *) hostinfo_nodejs->h_addr;
				sin_nodejs.sin_port = htons(PORT_NODEJS);
				sin_nodejs.sin_family = AF_INET;

				if(connect(sock_nodejs,(struct sockaddr *) &sin_nodejs, sizeof(struct sockaddr)) == SOCKET_ERROR)
				{
				    perror("Erreur avec la procedure connect() nodejs");
				    exit(errno);
				}

				/*
				 * Le code suivant effectue presque la meme chose que communicate mais pas encore tout a fait ;)
				 */


				//Reception de donnée depuis client
				//traitement,
					//Envoi de donnée au serveur nodejs
					//ou
					//Envoi de données a gmap
				//reception depuis ce dernier
				//envoi au client ou traitement puis envoi au client
			
/*
				do {
					memset(buffer_out, 0, TAILLE_BUFFER);
					size_in=recv(csock, buffer_in, TAILLE_BUFFER,0);
					if(size_in == 0) {
						printf("Client in disconnected");
						fflush(stdout);
					}
					else if(size_in == -1) {
						perror("Erreur avec la procedure recv() in ");
						exit(errno);
					}

printf("Buffer in : \n %s\n", buffer_in);
					if(send(sock_nodejs, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					printf("\nsize in : %d\n", size_in);


				} while (size_in>0&&size_in<TAILLE_BUFFER);
			
				memset(buffer_out, 0, TAILLE_BUFFER);
				while((size_out=recv(sock_nodejs, buffer_out, TAILLE_BUFFER-1,0))>0) {
				//if((size_out=recv(sock_nodejs, buffer_out, TAILLE_BUFFER-1,0))>0) {
printf("Buffer out : \n %s\n", buffer_out);
					if(send(csock, buffer_out, size_out, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}
				}	
	
				if(size_out == 0) {
					printf("Client out disconnected");
					fflush(stdout);
				}
				else if(size_out == -1) {
					perror("Erreur avec la procedure recv() out ");
					exit(errno);
				}		
*/

				/*
				 * A la place on utilise ca, on recupere le premier paquet on analyse et en fonction de ca on choisi
				 */

				/*
				 * On vide le buffer et on récupère le premier paquet.
				 */

				memset(buffer_out, 0, TAILLE_BUFFER);

				if((size_in=recv(csock, buffer_in, TAILLE_BUFFER,0))==-1) {
					perror("Erreur avec la procedure recv() in ");
					exit(errno);
				}

				/*
				 * On recopie les premiers caracteres du premier paquet pour filtrer la destination des requètes
				 */

				char petit_tampon[120];
				strncpy(petit_tampon,buffer_in,119);

				/*
				 * Début du filtrage, selon le host et la requete de destination on effectue une action différente :
				 * Cette partie est à optimiser pour diminuer la charge (identifier hostname, requete POST / GET)
				 *
				 * > Pour une requete contenant interface-infirmiere on va faire transiter les requetes vers nodejs
				 * L'important est de demander la racine / du serveur, nodejs répondra donc la page d'acceuil
				 *
				 * > Pour une requete contenant /INFIRMIERE on va faire effectuer différentes actions :
				 * Executer une application externe en CPP pour récupérer une URL qui devra être utilisé pour une requête à google map API.
				 * Puis générer une requete pour GMAPI
				 * Récupérer le fichier XML recu par Google en enlevant l'entête.
				 * Executer une application externe qui fera tourner l'optimisation RO sur ce fichier. L'application génère une page HTML.
				 * Le proxy doit lire cette page HTML générée et la transmettre au client.
				 *
				 * > Pour les autres requètes inconnues qui peuvent correspondre à des requetes type Asynchrone Javascript (Ajax), pour Google Map ...
				 * 
				 *
				 * Ces trois filtrages sont essentiels pour le bon fonctionnement de toutes les applications.
				 * Un simple prototype de chaque application sans être intégré ne permet de voir cette nécessité.
				 * De nombreuses modifications de ce type sont faites par la suite et divergent donc du sujet fourni.
				 * Toutes ces divergences sont essentielles pour le bon fonctionnement de toutes les applications.
				 */

				if(strstr(petit_tampon, chaine_intercept_inf)!=NULL) {

					/*
					 * On recoit une requete contenant /INFIRMIERE 
					 * Il faut extraire l'id et lancer l'application et récupérer ce qu'elle renvoie
					 */

					//printf("\nBuffer recu : \n %s \n \n", buffer_in);
					
//printf("\nChaine POST /INFIRMIERE interceptée\n\n");					

					s_id_post_inf=strstr(buffer_in,"id=");

printf("\nID selectionné : %s\n", s_id_post_inf);

					if(s_id_post_inf==NULL) {
						printf("\n Attention ! Pas d'id trouvé \n");
						exit(EXIT_FAILURE);
					}
					else {
						/*
						 * On extrait l'identifiant
						 */

						//On alloue l'espace necessaire pour l'id
						s_id_post_inf_comp=malloc(sizeof(char)*strlen(s_id_post_inf));
						//On copie l'id dans cette chaine
						strncpy(s_id_post_inf_comp,s_id_post_inf+3,strlen(s_id_post_inf));

						/*
						 * Premier appel de l'application externe, les actions suivantes découlent de ce premier appel
						 */

						premier_appel_app_ext(csock, sock_cacheujf, s_id_post_inf_comp);
					}
				}
				else if(strstr(petit_tampon, chaine_intercept_gest)!=NULL) {

//printf("\nChaine Interface infirmiere interceptée\n\n");					
					/*
					 * Requete relative a l'interface de gestion
					 * On envoie le premier paquet qui a été lu sur le socket nodejs
					 */

					if(send(sock_nodejs, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					/*
					 * Le premier paquet à été envoyé on souhaite maintenant que tout les paquets suivants
					 * transitent en I/O sur ces deux ports
					 */

					/*
					 * A modifier pour avoir une fonction qui gère les connexion persistantes et autres problèmes
					 * non pris en compte dans communicate.
					 */

					communicate(csock, sock_nodejs, buffer_in);
				}
				else {
					/*
					 * Les chaines recherchées n'ont pas été trouvées
					 * On envoie toutes les requetes sur cacheujf.
					 * Car il s'agit de requetes type Asynchrone Javascript (Ajax), pour Google Map ...
					 * ou destinées a un tout autre hôte/
					 * On envoie le premier paquet qui a été lu sur le socket cacheujf
					 */
					
//printf("\nChaine non reconnue redirection sur le cache ujf\n\n");					

					if(send(sock_cacheujf, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					/*
					 * Le premier paquet à été envoyé on souhaite maintenant que tout les paquets suivants
					 * transitent en I/O sur ces deux ports
					 */

					communicate(csock, sock_cacheujf, buffer_in);
				}

				/*
				 * Toutes les opérations sont finies le fils n'a plus d'actions a accomplir.
				 * Il peut fermer ses sockets et mourir.
				 */

				close(csock);
				close(sock_nodejs);
				close(sock_cacheujf);

				return 0;
			break;

			default :
				// Le père n'a pas besoin du socket ouvert pour la communication du fils
				close(csock);
			break;
		}
	}

	close(sock);

	return 0;
}

