/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 31 mars 2022, 10:45
 */

#include <Arduino.h>
#include <structures.h>
#include <sigfoxBallon.h>
#include <HardwareSerial.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typeDonnees lesDonnees;

SigfoxBallon Sig(27, 26, true);

double doubleEcrirePortSerie(){
    char *stop;
    char buffer[2] = "";
    char chaine[100] = "";
    double val;
    do {
        if (Serial.available() > 0) {
            buffer[0] = Serial.read();
            Serial.write(buffer[0]);
            strncat(chaine, buffer, 10);
        }
    } while (strncmp(buffer,  "\r",1) != 0);
    val = strtod(chaine, &stop);
    return val;
}

float floatEcrirePortSerie() {
    char buffer[2] = "";
    char chaine[100] = "";
    float val;
    do {
        if (Serial.available() > 0) {
            buffer[0] = Serial.read();
            Serial.write(buffer[0]);
            strncat(chaine, buffer, 10);
        }
    } while (strncmp(buffer, "\r",1) != 0);
    val = atof(chaine);
    return val;
}

void setup() {
    Serial.begin(115200);
    Sig.begin();
}

void loop() {
    // ecriture des valeur via liaison serie
    Serial.print("altitude : ");
    lesDonnees.altitude = floatEcrirePortSerie();
    Serial.print("latitude : ");
    lesDonnees.latitude = doubleEcrirePortSerie();
    Serial.print("longitude : ");
    lesDonnees.longitude = doubleEcrirePortSerie();
    Serial.print("radiation : ");
    lesDonnees.cpm = floatEcrirePortSerie();
    Serial.print("pression : ");
    lesDonnees.pression = floatEcrirePortSerie();
    Serial.print("humidite : ");
    lesDonnees.humidite = floatEcrirePortSerie();
    Serial.print("temperature : ");
    lesDonnees.temperature = floatEcrirePortSerie();
    Serial.print("\n");
    
    // test de la méthode coder trame de SigfoxBallon
    Sig.coderTrame(&lesDonnees);
    
    //Sig.envoyer();
    
    delay(3000);
}