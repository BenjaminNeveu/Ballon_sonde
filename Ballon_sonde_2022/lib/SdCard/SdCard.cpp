/* 
 * File:   SdCard.cpp
 * Author: bneveu
 * 
 * Created on 28 mars 2022, 08:56
 */

#include <structures.h>
#include "SdCard.h"

SdCard::SdCard(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, bool enDebug) {
    cs = csPin;
    debug = enDebug;
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
        if (debug == true) {
            Serial.println("Initialisation succes.");
        }
        retour = true;
    } else {
        if (debug == true) {
            Serial.println("Initialisation failed.");
        }
        retour = false;
    }
    return retour;
}

void SdCard::initFile(const char* path, const char* message) {
    File file = SDFileSystem.open(path, FILE_WRITE);
    
    if (!file) {
        if (debug == true) {
            Serial.print("Erreur ecriture du fichier / ");
        }
        return;
    } else {
        if (debug == true) {
            Serial.print("Succes ecriture du fichier / ");
        }
    }

    if (!file.print(message)) {
        if (debug == true) {
            Serial.println("Erreur ecriture de la premiere ligne");
        }
    } else {
        if (debug == true) {
            Serial.println("Succes ecriture de la premiere ligne");
        }
    }
    file.close();
}

void SdCard::ajouterChaine(const char* path, String chaine) {
    File file = SDFileSystem.open(path, FILE_APPEND);

    if (!file) {
        if (debug == true) {
            Serial.print("Erreur ouvertuverture fichier / ");
        }
        return;
    } else {
        if (debug == true) {
            Serial.print("Fichier ouvert avec succes / ");
        }
    }
    if (!file.println(chaine)) {
        if (debug == true) {
            Serial.println("Chaine pas ajoute avec succes");
        }
    } else {
        if (debug == true) {
            Serial.println("Chaine ajoute avec succes");
        }
    }
    file.close();
}

String SdCard::creerChaine(typeDonnees* lesDonnees) {
    String chaine;

    chaine = String(lesDonnees->annee) + "-";
    chaine += formatDateHeure(lesDonnees->mois) + "-";
    chaine += formatDateHeure(lesDonnees->jour) + " ";
    chaine += formatDateHeure(lesDonnees->heure) + ":";
    chaine += formatDateHeure(lesDonnees->minute) + ":";
    chaine += formatDateHeure(lesDonnees->seconde) + ";";
    chaine += String(lesDonnees->altitude, 2) + ";";
    chaine += String(lesDonnees->longitude, 8) + ";";
    chaine += String(lesDonnees->latitude, 8) + ";";
    chaine += String(lesDonnees->cpm, 2) + ";";
    chaine += String(lesDonnees->temperature, 2) + ";";
    chaine += String(lesDonnees->pression, 2) + ";";
    chaine += String(lesDonnees->humidite, 2);

    return chaine;
}

String SdCard::formatDateHeure(byte val) {
    String dateHeure;
    if (val < 10) {
        dateHeure = "0" + String(val);
    } else {
        dateHeure = String(val);
    }
    return dateHeure;
}