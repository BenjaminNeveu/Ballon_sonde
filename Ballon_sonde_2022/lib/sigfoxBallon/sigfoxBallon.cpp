/**
 * @file sigfoxBallon.cpp
 * @briel Implémentation de la classe SigfoxBallon
 * @version 1.0
 * @author Benjamin NEVEU
 * @date 18/03/2022
 * @details Classe de la trame SigFox 
 */

/* 
 * File:   sigfoxBallon.cpp
 * Author: bneveu
 * 
 * Created on 18 mars 2022, 11:38
 */

#include "sigfoxBallon.h"

/**
 * @brief SigfoxBallon::SigfoxBallon
 * @param rxPin broche rx de l'ESP32
 * @param txPin broche tx de l'ESP32
 * @param debugEn mode debug
 */
SigfoxBallon::SigfoxBallon(uint8_t rxPin, uint8_t txPin, bool debugEn) :
Sigfox(rxPin, txPin, debugEn) {

}

/**
 * @brief SigfoxBallon::coderTrame
 * @param lesDonnees donnée dans la trame
 */
void SigfoxBallon::coderTrame(typeDonnees *lesDonnees) {
    int bitSigneLongitude;
    int bitSigneTemperature;
    
    // recupération des valeur de la structure
    float altitude = lesDonnees->altitude;
    // offset de 40 pour la latitude
    double latitude = (lesDonnees->latitude - 40)*100000;
    double longitude = lesDonnees->longitude * 100000;
    float radiations = lesDonnees->cpm;
    float pression = lesDonnees->pression;
    float humidite = lesDonnees->humidite;
    float temperature = lesDonnees->temperature;
    
    // recuperation des bit de signe
    bitSigneLongitude = bitSigne(longitude);
    bitSigneTemperature = bitSigne(temperature);
    
    // verification de la valeur si elle se situe entre valMin et valMax
    altitude = verifValeur(altitude, 0, 32767);
    latitude = verifValeur(latitude, 0, 2097151);
    longitude = verifValeur(longitude, -1048575, 1048575);
    radiations = verifValeur(radiations, 0, 32767);
    pression = verifValeur(pression, 0, 1023);
    humidite = verifValeur(humidite, 0, 100);
    temperature = verifValeur(temperature, -63, 63);
    
    // arrondir les valeur
    altitude = arrondi(altitude);
    latitude = arrondi(latitude);
    longitude = arrondi(longitude);
    radiations = arrondi(radiations);
    pression = arrondi(pression);
    humidite = arrondi(humidite);
    temperature = arrondi(temperature);

    if (debug == true) {
        Serial.printf("altitude : %d\n", (int) altitude);
        Serial.printf("latitude : %d\n", (int) latitude);
        Serial.printf("bit de signe : %d\n", bitSigneLongitude);
        Serial.printf("longitude : %d\n", (int) longitude);
        Serial.printf("radiation : %d\n", (int) radiations);
        Serial.printf("pression : %d\n", (int) pression);
        Serial.printf("humidite : %d\n", (int) humidite);
        Serial.printf("bit de signe : %d\n", bitSigneTemperature);
        Serial.printf("temperature : %d\n", (int) temperature);
        Serial.printf("\n");
    }

    //----------------formater-trame--------------------

    byte octet; //variable sur un octet (= 8bits)
    byte coteGauche;
    byte coteDroit;
    int i = 11; //indice du tableau (on commence à la fin du tableau)

    //trame[11]
    coteDroit = (int) temperature;
    coteGauche = (int) humidite << 7;
    octet = coteDroit | coteGauche;

    if (bitSigneTemperature == 1) {
        octet = octet | 0x40;
    } else {
        octet = octet & 0xBF;
    }
    trame[i] = octet;
    i--;

    //trame[10]
    coteDroit = (int) humidite >> 1;
    coteGauche = (int) pression << 6;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;

    //trame[9]
    octet = (int) pression >> 2;
    trame[i] = octet;
    i--;

    //trame[8]
    octet = radiations;
    trame[i] = octet;
    i--;

    //trame[7]
    coteDroit = (int) radiations >> 8;
    coteGauche = (int) longitude << 7;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;

    //trame[6]
    octet = (int) longitude >> 1;
    trame[i] = octet;
    i--;

    //trame[5]
    octet = (int) longitude >> 9;
    trame[i] = octet;
    i--;

    //trame[4]
    coteDroit = (int) longitude >> 17;
    coteGauche = (int) latitude << 4;
    octet = coteDroit | coteGauche;

    if (bitSigneLongitude == 1) {
        octet = octet | 0x08;
    } else {
        octet = octet & 0xF7;
    }
    trame[i] = octet;
    i--;

    //trame[3]
    octet = (int) latitude >> 4;
    trame[i] = octet;
    i--;

    //trame[2]
    octet = (int) latitude >> 12;
    trame[i] = octet;
    i--;

    //trame[1]
    coteDroit = (int) latitude >> 20;
    coteGauche = (int) altitude << 1;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;

    //trame[0]
    octet = (int) altitude >> 7;
    trame[i] = octet;
    i--;

    if (debug == true) {
        for (i = 0; i < 12; i++) {
            Serial.print(trame[i] < 16 ? "0" : "");
            Serial.print(trame[i], HEX);
        }
        Serial.print("\n");
        Serial.print("Trame codée\t");
    }
}

/**
 * 
 * @param val
 * @return 
 */
float SigfoxBallon::arrondi(float val) {
    int digit;
    // décalage de la virgule de 10^1
    val = val * 10;
    // recuperer le dernier digit
    digit = (int) val % 10;
    // décalage de la virgule de 10^-1
    val = val / 10;
    if (digit >= 5) {
        //Si digit >= 5, val+1
        val = val + 1;
    }
    return val;
}

/**
 * 
 * @param val
 * @return 
 */
double SigfoxBallon::arrondi(double val) {
    int digit;
    // décalage de la virgule de 10^1
    val = val * 10;
    // recuperer le dernier digit
    digit = (int) val % 10;
    // décalage de la virgule de 10^-1
    val = val / 10;
    if (digit >= 5) {
        //Si digit >= 5, val+1
        val = val + 1;
    }
    return val;
}

/**
 * 
 * @param val
 * @param valMin
 * @param valMax
 * @return 
 */
float SigfoxBallon::verifValeur(float val, float valMin, float valMax) {
    if (val > valMax) {
        val = valMax;
    }
    if (val < 0) {
        if (val < valMin) {
            val = valMin * -1;
        } else {
            val = val * -1;
        }
    }
    return val;
}

/**
 * 
 * @param val
 * @param valMin
 * @param valMax
 * @return 
 */
double SigfoxBallon::verifValeur(double val, double valMin, double valMax) {
    if (val > valMax) {
        val = valMax;
    }
    if (val < 0) {
        if (val < valMin) {
            val = valMin * -1;
        } else {
            val = val * -1;
        }
    }
    return val;
}

/**
 * 
 * @param val
 * @return 
 */
int SigfoxBallon::bitSigne(float val) {
    int bitSigne;
    if (val >= 0) {
        bitSigne = 0;
    } else {
        bitSigne = 1;
    }
    return bitSigne;
}

/**
 * 
 * @param val 
 * @return 
 */

int SigfoxBallon::bitSigne(double val) {
    int bitSigne;
    if (val >= 0) {
        bitSigne = 0;
    } else {
        bitSigne = 1;
    }
    return bitSigne;
}

