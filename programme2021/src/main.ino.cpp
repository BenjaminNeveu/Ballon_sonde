/**
 * @file main.ino.cpp
 * @brief main du projet
 * @version 1.0
 * @author Sofiane SABRI
 * @date 20/05/2021
 * @details Initialisation des différentes taches
 */
#include "structures.h"
#include "taches.h"
typeDonnees lesDonnees;

void setup() {

    Serial.begin(SERIAL_BAUD);
    
    
    
    Taches *nouvelleTache = new Taches();

    xTaskCreate(
            nouvelleTache->tacheBME280, /* Task function. */
            "tacheBME280", /* name of task. */
            40000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

    xTaskCreate(
            nouvelleTache->tacheRadiations, /* Task function. */
            "tacheRadiations", /* name of task. */
            40000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

    xTaskCreate(
            nouvelleTache->tacheGPS, /* Task function. */
            "tacheGPS", /* name of task. */
            40000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            2, /* priority of the task */
            NULL); /* Task handle to keep track of created task */

//   xTaskCreate(
//            nouvelleTache->tacheAffichage, /* Task function. */
//            "tacheAffichage", /* name of task. */
//            40000, /* Stack size of task */
//            &lesDonnees, /* parameter of the task */
//            1, /* priority of the task */
//            NULL); /* Task handle to keep track of created task */
    
        xTaskCreate(
            nouvelleTache->tacheSigfox, /* Task function. */
            "tacheSigfox", /* name of task. */
            40000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            2, /* priority of the task */
            NULL); /* Task handle to keep track of created task */
//        
        xTaskCreate(
            nouvelleTache->tacheCarteSD, /* Task function. */
            "tacheCarteSD", /* name of task. */
            40000, /* Stack size of task */
            &lesDonnees, /* parameter of the task */
            1, /* priority of the task */
            NULL); /* Task handle to keep track of created task */


}

void loop() {

}
