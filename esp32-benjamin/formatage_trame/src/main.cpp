/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 18 mars 2022, 11:11
 */

#include <Arduino.h>
#include <sigfox.h>
#include <structures.h>
typeDonnees lesDonnees;

int LED = 22;

Sigfox Sig(27, 26, true);

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    Serial.println("Setup done");
    
    Sig.begin();
    
    lesDonnees.DonneesCapteurs.cpm = 100.81;
    lesDonnees.DonneesCapteurs.humidite = 85.60;
    lesDonnees.DonneesCapteurs.pression = 1000.14;
    lesDonnees.DonneesCapteurs.temperature = -15.25;
    lesDonnees.position.altitude = 28056.25;
    lesDonnees.position.latitude = 47.089909;
    lesDonnees.position.longitude = -1.270549;
    
    Sig.coderTrame(& lesDonnees);
    Sig.envoyer(Sig.trame,sizeof (Sig.trame));
}

void loop() {
    
    digitalWrite(LED,digitalRead(LED) ^1);
    delay(200);
    digitalWrite(LED,digitalRead(LED) ^1);
    delay(1000);
    
}