#include <iostream>
#include <stdlib.h>
#include "FromGoogleMapXMLToDistanceTable.h"

// Constructeur
FromGoogleMapXMLToDistanceTable::FromGoogleMapXMLToDistanceTable() : LwSaxParser() {
    state = UNKNOWN;
    adresseCourante = "";
    adresses = NULL;
    ligne = NULL;
    distanceMatrix = NULL;
    addrTemp = "";
}


FromGoogleMapXMLToDistanceTable::~FromGoogleMapXMLToDistanceTable() {
    
}


std::vector<std::string> * FromGoogleMapXMLToDistanceTable::getAdresses() {
    // Ici, on triche pour que le test fonctionne du premier coup
    // Ces lignes sont à remplacer par un vrai parcours du document XML
    // Cette méthode ne devra plus contenir que return adresses; .
  /*  adresses = new std::vector<std::string>();
    adresses->push_back("46 Avenue Félix Viallet, IUT 2, 38031 Grenoble, France");
    adresses->push_back("Rond-point de la Croix de Vie, 38700 La Tronche, France");
    adresses->push_back("Rue Casimir Brenier, 38000 Grenoble, France");
    adresses->push_back("25 Rue des Martyrs, 38000 Grenoble, Francev");
 */
    return adresses;
}

std::vector< std::vector<int> > * FromGoogleMapXMLToDistanceTable::getDistances() {
    // Ici, on triche pour que le test fonctionne du premier coup
    // Ces lignes sont à remplacer par un vrai parcours du document XML
    // Cette méthode ne devra plus contenir que return distanceMatrix; .
/*
    distanceMatrix = new std::vector< std::vector<int> >();
    
    ligne = new std::vector<int>();    
    ligne->push_back(0);
    ligne->push_back(3430);
    ligne->push_back(224);
    ligne->push_back(3000);
    std::vector<int> ligne0;
    for (int i = 0; i < ligne->size(); i++) {
        ligne0.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne0);
    delete ligne;
    
    
    ligne = new std::vector<int>();    
    ligne->push_back(3345);
    ligne->push_back(0);
    ligne->push_back(3569);
    ligne->push_back(5625);
    std::vector<int> ligne1;
    for (int i = 0; i < ligne->size(); i++) {
        ligne1.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne1);
    delete ligne;
    
    ligne = new std::vector<int>();    
    ligne->push_back(310);
    ligne->push_back(3337);
    ligne->push_back(0);
    ligne->push_back(2907);
    std::vector<int> ligne2;
    for (int i = 0; i < ligne->size(); i++) {
        ligne2.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne2);
    delete ligne;
    
    ligne = new std::vector<int>();    
    ligne->push_back(2543);
    ligne->push_back(5569);
    ligne->push_back(2326);
    ligne->push_back(0);
    std::vector<int> ligne3;
    for (int i = 0; i < ligne->size(); i++) {
        ligne3.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne3);
    delete ligne;
    
    dsg
 */
    return distanceMatrix;
    
}

void FromGoogleMapXMLToDistanceTable::on_start_document() {
    state = START;
    adresses = new std::vector<std::string>();
    distanceMatrix = new std::vector< std::vector<int> >();
}


void FromGoogleMapXMLToDistanceTable::on_end_document() {
    state = UNKNOWN;
}
void FromGoogleMapXMLToDistanceTable::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
    switch(state)
    {
        case START:
        {
            if (name =="origin_address")
            {
                state = ORIGIN_ADDRESS;
                addrTemp ="";
            }
            if (name =="row")
            {
                state = ROW;
                ligne = new std::vector<int>();
            }
            break;
        }
        case ROW:
        {
            if (name =="element")
            {
                state = ELEMENT;
            }
            break;
        }
        case ELEMENT:
        {
            if (name =="distance")
            {
                state = DISTANCE;
            }
            break;
        }
        case DISTANCE:
        {
            if (name =="value")
            {
                state = VALUE;
            }
            break;
        }
        default:
        {

        break;
        }
    }
}
void FromGoogleMapXMLToDistanceTable::on_characters(const Glib::ustring& text)
{
        switch(state)
        {
            case ORIGIN_ADDRESS:{
                addrTemp +=text;
                break;
            }
            case VALUE:{
                ligne->push_back(atoi(text.c_str()));
                break;
            }
            default:{
                break;
            }
        }
 }
void FromGoogleMapXMLToDistanceTable::on_end_element(const Glib::ustring& name)
{
    switch(state)
    {
        case ORIGIN_ADDRESS:{
            adresses->push_back(addrTemp);
            state = START;
            break;
        }
        case VALUE:{
            state = DISTANCE;
            break;
            }
        case DISTANCE:{
            if(name =="distance")
                state = ELEMENT;
            break;
            }
        case ELEMENT:{
            if(name=="element")
                state = ROW;
            break;
        }
        case ROW:{
            state = START;
            std::vector<int> ligne0;
            for (int i = 0; i < ligne->size(); i++) {
                ligne0.push_back(ligne->at(i));
            }
            distanceMatrix->push_back(ligne0);
            delete ligne;
            break;
        }
        default:{
        break;
        }
    }
}
// enum PossibleStates {START, ORIGIN_ADDRESS, ROW, ELEMENT, DISTANCE, VALUE, UNKNOWN};
// A compléter en implémentant les méthodes à redéfinir de LwSaxParser

