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
