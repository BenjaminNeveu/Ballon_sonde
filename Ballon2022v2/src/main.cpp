/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 31 mars 2022, 10:45
 */

#include <Arduino.h>
#include <structures.h>
#include <taches.h>

typeDonnees lesDonnees;

void setup() {

    Serial.begin(115200);
    Serial.println("Setup done");
    Wire.begin(21, 22);

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
            newTask->tacheBME280, /* Task function. */
            "tacheBME280", /* name of task. */
            10000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

    xTaskCreate(
            newTask->tacheRadiation, /* Task function. */
            "tacheRadiation", /* name of task. */
            10000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */
    
    xTaskCreate(
            newTask->tacheGPS,
            "tacheGPS",
            10000,
            &lesDonnees,
            1,
            NULL);
    
    xTaskCreate(
            newTask->tacheSigfox, /* Task function. */
            "tacheSigfox", /* name of task. */
            10000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            2, /* priority of the task */
            NULL); /* Task handle to keep track of created task */
    
    xTaskCreate(
            newTask->tacheCarteSd, /* Task function. */
            "tacheCarteSd", /* name of task. */
            10000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

    xTaskCreate(
            newTask->tacheServeurWeb, /* Task function. */
            "tacheServeurWeb", /* name of task. */
            10000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            2, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

}

void loop() {  
    delay(100);
}