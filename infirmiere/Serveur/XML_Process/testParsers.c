#include <iostream>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdio.h>
#include "FromXMLToGoogleMapHTTPRequest.h"
#include "FromGoogleMapXMLToDistanceTable.h"
#include "SortVisits.h"


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Veuillez entrer le nom du fichier à analyser en paramètre de la commande : testParsers filename.xml id" << std::endl;
        std::cout << "Exemple: " << std::endl;
        std::cout << "> ./testParsers data/cabinetInfirmier.xml 001" << std::endl << std::endl;
        return 1;
    }
    std::string file ="data/cabinetInfirmier.xml";
    char *filename = &file[0];
    //char * filename = file.c_str();

    char * id = argv[2];
    int idInt = 2;
    
    SortVisits exec;
    FromXMLToGoogleMapHTTPRequest dataBaseParser(id);
   // FromGoogleMapXMLToDistanceTable googleMapParser;
   // std::vector<std::string> * adresses;
   // std::vector< std::vector<int> > * distances;

    
    int option = atoi(argv[1]);
    /*while (option != 0) {
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Que voulez-vous faire avec ce fichier ?" << std::endl;
        std::cout << "0 quitter l'application" << std::endl;
        std::cout << "1 obtenir la requête HTTP à envoyer à GoogleMap pour récupérer les matrices de distances entre les adresses " << std::endl;
        std::cout << "2 créer un tableau c++ à partir du fichier XML renvoyé par GoogleMap " << std::endl;
        std::cout << "Veuillez taper 0, 1, ou 2" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
        
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), std::cin.widen ( '\n' ));
            option = -1;
        }
      */
        switch (option) {
            case 0: // On ne fait rien et l'on s'en va...
                break;
                
            case 1: // obtenir la requête HTTP à envoyer à GoogleMap pour récupérer les matrices de distances entre les adresses
                dataBaseParser.parseDocument(filename);
                std::cout << dataBaseParser.getGoogleMapHttpRequest()<< std::endl;
                break;
                
            case 2: // créer un tableau c++ à partir du fichier XML renvoyé par GoogleMap
                    dataBaseParser.parseDocument(filename);
                //creation du tableau
                    exec.processDistanceMatrix(filename,"data/pageCabinet.html",atoi(id),"data/reponseGoogle.xml");
                break;
         }
    }
//}
               /* googleMapParser.parseDocument("data/reponseGoogle.xml");
                std::cout << std::endl;
                std::cout << std::endl;
                adresses = googleMapParser.getAdresses();
                distances = googleMapParser.getDistances();

                
                // écrire la liste des adresses:
                for (unsigned int i = 0; i < adresses->size(); i++)
                {
                    std::cout << "| " << i << " | " << adresses->at(i) << " | " << std::endl;
                }
                std::cout << std::endl;
                std::cout << std::endl;
                
                
                // écrire le tableau des distances avec les index
                std::cout << "     | ";
                for(unsigned int c = 0; c < adresses->size(); c++) {
                    std::string spacesBefore = "";
                    if (c < 10)
                        spacesBefore = "   ";
                    else if (c < 100)
                        spacesBefore = "  ";
                    else if (c < 1000)
                        spacesBefore = " ";
                    else
                        spacesBefore = "";
                    
                    std::cout << spacesBefore << c <<  " | ";
                }
                std::cout << std::endl << "  --------------------------------" << std::endl;
                


                for (unsigned int ligne = 0; ligne < adresses->size(); ligne++)
                {
                    std::cout << "   " << ligne << " | ";
                    for (unsigned int colonne = 0; colonne < adresses->size(); colonne++)
                    {

                        int value = (distances->at(ligne)).at(colonne);

                        std::string spacesBefore = "";

                        if (value < 10)
                            spacesBefore = "   ";
                        else if (value < 100)
                            spacesBefore = "  ";
                        else if (value < 1000)
                            spacesBefore = " ";
                        else
                            spacesBefore = "";
                        
                        std::cout << spacesBefore << value << " | ";
                    }
                    std::cout << std::endl << "  --------------------------------" << std::endl;
                }
                
                break;
                
                    
            default:
                option = -1;
                break;
        }
        std::cout << std::endl << "-------------------------------------------" << std::endl << std::endl;

 }
	
	return 0;
}
*/
