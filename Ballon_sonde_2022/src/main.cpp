/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 31 mars 2022, 10:45
 */

/*
 * Bibliothèque : TinyGPS @ 0.0
 * installation : pio lib -g install 416
 * 
 * Bibliothèque : BME280 @ 3.0.0
 * installation : pio lib -g install 901
 * 
 * Bibliothèque : radiationwatch @ 0.6.4
 * installation : pio lib -g install 1523
 * 
 * Bibliothèque : ESPAsyncWebServer @ 1.2.3
 * Bibliothèque : ESPAsyncTCP @ 1.2.2
 * Bibliothèque : AsyncTCP @ 1.1.1
 * installation : pio lib -g install 306
 * 
 * Bibliothèque : ArduinoJson @ 6.19.3
 * installation : pio lib -g install 64
 */

#include <Arduino.h>
#include <structures.h>
#include <taches.h>

typeDonnees lesDonnees;

void setup() {

    Serial.begin(115200);
    Serial.println("Setup done");
    Wire.begin(21, 22);
    
    delay(1000);

    lesDonnees.annee = 0;
    lesDonnees.mois = 0;
    lesDonnees.jour = 0;

    lesDonnees.heure = 0;
    lesDonnees.minute = 0;
    lesDonnees.seconde = 0;

    lesDonnees.altitude = 0.0;
    lesDonnees.longitude = 0.0;
    lesDonnees.latitude = 40.0;
    
    Taches *newTask = new Taches();

    xTaskCreate(
            newTask->tacheBME280,
            "tacheBME280",
            10000, 
            &lesDonnees, 
            1,
            NULL);

    xTaskCreate(
            newTask->tacheRadiation,
            "tacheRadiation",
            10000,
            &lesDonnees,
            1,
            NULL);
   
    xTaskCreate(
            newTask->tacheGPS,
            "tacheGPS",
            10000,
            &lesDonnees,
            1,
            NULL);
   
    xTaskCreate(
            newTask->tacheCarteSd,
            "tacheCarteSd",
            10000,
            &lesDonnees,
            1,
            NULL);

    xTaskCreate(
            newTask->tacheSigfox,
            "tacheSigfox",
            10000,
            &lesDonnees,
            2,
            NULL);
    
    xTaskCreate(
            newTask->tacheServeurWeb,
            "tacheServeurWeb", 
            10000, 
            &lesDonnees, 
            2, 
            NULL);
    
    xTaskCreate(
            newTask->tacheAfficher,
            "tacheAfficher", 
            10000, 
            &lesDonnees, 
            2, 
            NULL);
}

void loop() {  
    delay(100);
}
