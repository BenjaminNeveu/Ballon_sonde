/* 
 * File:   sigfoxBallon.cpp
 * Author: bneveu
 * 
 * Created on 18 mars 2022, 11:38
 */

#include "sigfoxBallon.h"

SigfoxBallon::SigfoxBallon(uint8_t rxPin, uint8_t txPin, bool debugEn):
    Sigfox(rxPin, txPin, debugEn)
{

}

void SigfoxBallon::coderTrame(typeDonnees *lesDonnees) {

    int bitSigneLongitude;
    int bitSigneTemperature;

    int altitude = lesDonnees->position.altitude;
    double latitude = (lesDonnees->position.latitude - 40)*1000000;
    double longitude = lesDonnees->position.longitude * 1000000;
    float radiations = lesDonnees->DonneesCapteurs.cpm * 10;
    float pression = lesDonnees->DonneesCapteurs.pression * 10;
    float humidite = lesDonnees->DonneesCapteurs.humidite * 10;
    float temperature = lesDonnees->DonneesCapteurs.temperature * 10;

    Serial.printf("altitude : %d\n", altitude);

    latitude = arrondi(latitude);
    Serial.printf("latitude : %d\n", (int) latitude);

    if (longitude >= 0) {
        bitSigneLongitude = 0;
    } else {
        longitude = longitude * -1;
        bitSigneLongitude = 1;
    }

    longitude = arrondi(longitude);
    Serial.printf("bit de signe : %d\n", bitSigneLongitude);
    Serial.printf("longitude : %d\n", (int) longitude);

    radiations = arrondi(radiations);
    Serial.printf("radiation : %d\n", (int) radiations);

    pression = arrondi(pression);
    Serial.printf("pression : %d\n", (int) pression);

    humidite = arrondi(humidite);
    Serial.printf("humidite : %d\n", (int) humidite);

    if (temperature >= 0) {
        bitSigneTemperature = 0;
    } else {
        temperature = temperature * -1;
        bitSigneTemperature = 1;
    }

    temperature = arrondi(temperature);
    Serial.printf("bit de signe : %d\n", bitSigneTemperature);
    Serial.printf("temperature : %d\n", (int) temperature);

    Serial.printf("\n");

    //----------------formater-trame--------------------

    byte octet; //variable sur un octet (= 8bits)
    byte coteGauche;
    byte coteDroit;
    int i = 11; //indice du tableau (on commence à la fin du tableau)

    //trame[11]
    coteDroit = temperature;
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
    coteGauche = altitude << 1;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;

    //trame[0]
    octet = altitude >> 7;
    trame[i] = octet;
    i--;
}

float SigfoxBallon::arrondi(float val) {
    int digit;
    // recuperer le dernier digit
    digit = (int)val % 10;
    if (digit >= 5) {
        //Si digit >= 5, val+1
        val = val / 10 + 1;
    } else {
        //Si digit >= 5, val+0
        val = val / 10;
    }
    return val;
}

double SigfoxBallon::arrondi(double val) {
    int digit;
    // recuperer le dernier digit
    digit =(int)val % 10;
    if (digit >= 5) {
        //Si digit >= 5, val+1
        val = val / 10 + 1;
    } else {
        //Si digit >= 5, val+0
        val = val / 10;
    }
    return val;
}