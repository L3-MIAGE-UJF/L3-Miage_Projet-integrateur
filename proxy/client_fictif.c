#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {
	int port = 80;
	int indice;

	/**
	 * Récupération des paramètres, numéro de port ...
	 */
	
	/*
	 * Création des structures, sockets, ...
	 */

	//Créatin de la socket d'écoute
	socket();

	//Attachement de la socket
	bind();

	//construction adresse serveur
	gethostbyname();

	//Ouverture du service
	listen();
	
	while(1) {
		/*
		 * En attente de connexion
		 */
		accept();

		//Création d'un processus fils
		fork();

		switch(pid_fils) {
			case 0 :
				/*
				 * Analyse de la requète
				 */
				//dialogue avec le client

				return 0;
				//exit(0);
			break;

			default :
			break;
		}
	}

	return 0;
}

