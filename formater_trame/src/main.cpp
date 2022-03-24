/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 18 mars 2022, 11:11
 */

#include <Arduino.h>
#include <sigfox.h>
#include <structures.h>
#include <sigfoxBallon.h>

typeDonnees lesDonnees;

int LED = 22;

SigfoxBallon Sig(27, 26, true);

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    
    Sig.begin();
    
    Serial.println("Setup done");
    
    lesDonnees.DonneesCapteurs.cpm = 4800.81;
    lesDonnees.DonneesCapteurs.humidite = 90.60;
    lesDonnees.DonneesCapteurs.pression = 624.14;
    lesDonnees.DonneesCapteurs.temperature = -20.25;
    lesDonnees.position.altitude = 28056.25;
    lesDonnees.position.latitude = 47.089909;
    lesDonnees.position.longitude = -1.300549;
    
    Sig.coderTrame(&lesDonnees);
    Sig.envoyer(Sig.trame,sizeof (Sig.trame));
}

void loop() {
    
    digitalWrite(LED,digitalRead(LED) ^1);
    delay(200);
    digitalWrite(LED,digitalRead(LED) ^1);
    delay(1000);
    
}