/* 
 * File:   SdCard.cpp
 * Author: bneveu
 * 
 * Created on 28 mars 2022, 08:56
 */

#include <structures.h>
#include "SdCard.h"

SdCard::SdCard(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin) {
    cs = csPin;
}

SdCard::SdCard(const SdCard& orig) {
}

SdCard::~SdCard() {
}

bool SdCard::begin() {
    bool retour;
    SPI.begin(14, 2, 15, cs);
    // L'instance SDFileSystem de la classe SDFS est créé dans le fichier inclus SD.h
    if (SDFileSystem.begin(cs)) {
        Serial.println("Initialisation succes.");
        retour = true;
    } else {   
        Serial.println("Initialisation failed.");
        retour = false;
    }
    return retour;
}

void SdCard::initFile(const char* path, const char* message) {
    File file = SDFileSystem.open(path, FILE_WRITE);
    
    if (!file) {
        Serial.println("Erreur ecriture du fichier");
        return;
    }else{
        Serial.println("Succes ecriture du fichier");
    }
    
    if (!file.print(message)) {
        Serial.println("Erreur ecriture de la premiere ligne");
    }else{
        Serial.println("Succes ecriture de la premiere ligne");
    }
    file.close();
}


void SdCard::ajouter(const char* path, String chaine) {
    File file = SDFileSystem.open(path, FILE_APPEND);
    
    if (!file) {
        Serial.println("Erreur ouvertuverture fichier");
        return;
    }else{
        Serial.println("Fichier ouvert avec succes");
    }
    if (!file.println(chaine)) {
        Serial.println("Chaine pas ajoute avec succes");
    }else{
        Serial.println("Chaine ajoute avec succes");
    }
    file.close();
}

void SdCard::creerChaine(typeDonnees* lesDonnees) {
    chaine = String(lesDonnees->annee) + "-";
    chaine += formatDateHeure(lesDonnees->mois) + "-";
    chaine += formatDateHeure(lesDonnees->jour) + " ";
    chaine += formatDateHeure(lesDonnees->heure) + ":";
    chaine += formatDateHeure(lesDonnees->minute) + ":";
    chaine += formatDateHeure(lesDonnees->seconde) + ";";
    chaine += String(lesDonnees->altitude) + ";";
    chaine += String(lesDonnees->latitude,6) + ";";
    chaine += String(lesDonnees->longitude,6) + ";";
    chaine += String(lesDonnees->cpm) + ";";
    chaine += String(lesDonnees->pression) + ";";
    chaine += String(lesDonnees->temperature) + ";";
    chaine += String(lesDonnees->humidite);
} 

String SdCard::formatDateHeure(byte val) {
    String dateHeure;
    if ( val < 10 ){
        dateHeure = "0" + String(val);
    }else{
        dateHeure = String (val); 
    }
    return dateHeure;
}