#include <iostream>

#include <curl/curl.h>
#include "Adresse.h"
#include "FromXMLToGoogleMapHTTPRequest.h"

FromXMLToGoogleMapHTTPRequest::FromXMLToGoogleMapHTTPRequest(std::string id) : LwSaxParser()
{
    this->id = "00" + id;

    request = "";
    listeAdresses = "";
    state = OTHER;
    adresseCabinet = false;
}

FromXMLToGoogleMapHTTPRequest::~FromXMLToGoogleMapHTTPRequest() {

}

std::string FromXMLToGoogleMapHTTPRequest::getGoogleMapHttpRequest() {
    request = "https://maps.googleapis.com/maps/api/distancematrix/xml?origins=" + std::string(curl_easy_escape(NULL,listeAdresses.c_str(),0)) + "&destinations=" + std::string(curl_easy_escape(NULL,listeAdresses.c_str(),0));
    return request;
}

std::string FromXMLToGoogleMapHTTPRequest::findAttribute(const AttributeList& attributeList, std::string attributeName) {
    std::string resultat = "";
    xmlpp::SaxParser::AttributeList::const_iterator iter = attributeList.begin();
    while ((iter != attributeList.end()) && (iter->name != attributeName)) {
        iter++;
    }
    if (iter != attributeList.end()) {
        resultat = iter->value;
    }
    return resultat;
}

// A compléter en implémentant les méthodes à redéfinir de LwSaxParser

void FromXMLToGoogleMapHTTPRequest::on_start_document() {
    state = START;
}

void FromXMLToGoogleMapHTTPRequest::on_end_document() {
    state = OTHER;
}
void FromXMLToGoogleMapHTTPRequest::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{

    switch(state)
    {
        case START:
        {
            if (name =="ns1:adresse")
            {
                adresseCourante = new Adresse();
                state = ADRESSE;
            }
            if(name =="ns1:visite")
            {
                state=VISITE;
                if(findAttribute(attributes,"intervenant")==id)
                    {
                        listeAdresses+= "|" + adresseCourante -> getGoogleAdresse();
                    }

            }
            break;
        }
        case ADRESSE:
        {
            if (name == "ns1:numero")
                state = NUMERO;
            if (name == "ns1:rue")
                state = RUE;
            if (name == "ns1:ville")
                state = VILLE;
            if (name == "ns1:codePostal")
                state = CODEPOSTAL;
            if (name == "ns1:etage")
                state = ETAGE;
            break;
        }
    case VISITE:{


                break;
        }
    default :{
        break;}
   }
}

void FromXMLToGoogleMapHTTPRequest::on_characters(const Glib::ustring& text)
{
    switch(state)
    {
        case NUMERO:
        {
            adresseCourante -> setNumero(text);
        break;
        }
        case RUE:
        {
            adresseCourante -> setRue(text);
        break;
        }
        case VILLE:
        {
            adresseCourante -> setVille(text);
        break;
        }
        case CODEPOSTAL:
        {
            adresseCourante -> setCodePostal(text);
        break;
        }
        default :
        {
        break;
        }
    }
}

void FromXMLToGoogleMapHTTPRequest::on_end_element(const Glib::ustring& name)
{
    switch(state)
    {
    case NUMERO:{
        state = ADRESSE;
        break;}
    case RUE:{
        state = ADRESSE;
        break;}
    case VILLE:{
        state = ADRESSE;
        break;}
    case CODEPOSTAL:{
        state = ADRESSE;
        break;}
    case ETAGE:{
        state = ADRESSE;
    break;}
    case ADRESSE:{
        if(!adresseCabinet)
        {
            listeAdresses+=adresseCourante -> getGoogleAdresse();
            adresseCabinet = true;
        }
        state = START;
        break;}
    case VISITE:{
        state = START;
        break;}
    default :{
        break;}
    }


}














