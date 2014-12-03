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

#include "libcomm/communicate.h"

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#define TAILLE_BUFFER 128

int main(int argc, char * argv[]) {

	int port = 80;
	int indice;

	int sock;
	struct sockaddr_in sin = { 0 };
	struct sockaddr_in csin = { 0 };

	struct hostent *hostinfo = NULL;
	
	const char *hostname = "127.0.0.1";

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

	if((sock = socket(AF_INET, SOCK_STREAM, 0))==SOCKET_ERROR) {
	    perror("Erreur avec la procedure socket()");
	    exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;

	if(bind (sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
	    perror("Erreur avec la procedure bind()");
	    exit(errno);
	}

	; /* on récupère les informations de l'hôte auquel on veut se connecter */
	if ((hostinfo = gethostbyname(hostname))==NULL) {
	    fprintf (stderr, "Unknown host %s.\n", hostname);
	    exit(EXIT_FAILURE);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(PORT); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;

	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
	    perror("connect()");
	    exit(errno);
	}

	close(sock);

	return 0;
}

