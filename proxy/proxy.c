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


#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#define TAILLE_BUFFER 128

int main(int argc, char * argv[]) {
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
	//typedef struct in_addr IN_ADDR;

	int port = 80;
	int indice;

	int pid_fils;

	SOCKET sock;

	SOCKADDR_IN sin = { 0 };
	SOCKADDR_IN csin = { 0 };

	SOCKET csock;

	socklen_t sinsize = sizeof csin;

	//Buffers envoi / reception

	char buffer_in[TAILLE_BUFFER];
	char buffer_out[TAILLE_BUFFER];
	int size_in;//, size_out;

	/**
	 * Récupération des paramètres, numéro de port ...
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
	
	/*
	 * Création des structures, sockets, ...
	 */

	//Créatin de la socket d'écoute

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == INVALID_SOCKET) {
	    perror("Erreur avec la procedure socket()");
	    exit(errno);
	}


	//Attachement de la socket

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

	sin.sin_family = AF_INET;

	sin.sin_port = htons(port);

	if(bind (sock, (SOCKADDR *) &sin, sizeof(sin)) == SOCKET_ERROR) {
	    perror("Erreur avec la procedure bind()");
	    exit(errno);
	}

	//Ouverture du service
	if(listen(sock, 5) == SOCKET_ERROR) { // Limité a 5 connexions simultanées
	    perror("Erreur avec la procedure listen()");
	    exit(errno);
	}

	while(1) {
		/*
		 * En attente de connexion
		 */
		csock = accept(sock, (SOCKADDR *) &csin, &sinsize);

		if(csock == INVALID_SOCKET) {
		    perror("Erreur avec la procedure accept()");
		    exit(errno);
		}

		//Création d'un processus fils
		pid_fils=fork();

		switch(pid_fils) {
			case 0 :
				printf("fils\n");
				close(sock);
				
				/*
				 * Analyse de la requète
				 */
				
				//dialogue avec le client
				while((size_in=recv(csock, buffer_in, TAILLE_BUFFER,0))>0) {

					snprintf(buffer_out, size_in, "%s", buffer_in);

					printf("Buffer in : \n %s\n", buffer_in);

					if(send(csock, buffer_out, size_in,0)<0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}
				}

				if(size_in<0) {
					perror("Erreur avec la procedure recv()");
					exit(errno);
				}			

				close(csock);
				//die();
				return 0;
			break;

			default :
				printf("pere\n");
				close(csock);
			break;
		}
	}

	close(sock);

	return 0;
}

