/**
 @file   main.cpp
 @brief 
 @version 1.0
 @author cmaillard
 @date 30/03/2022
 */

#include <Arduino.h>
#include <Taches.h>




typeDonnees lesDonnees;



/**
 * 
 */
void setup() {


    Serial.begin(115200); // initialisation de al liaison serie avec uen vitesse de 115200
    Wire.begin(21, 22); // redefition des bornes SDA et SCL pour la carte ballon ,sinon prblm avec la led qui est regle sur le bus I2C
    

    // initialisation d'un pointeur pour utilise les taches avec uen classe
    Taches *newTask = new Taches();

    // creation d'une tache
    xTaskCreate(
            newTask->TacheBME,
            "TacheBME",
            10000,
            &lesDonnees,
            1,
            NULL);

    xTaskCreate(
            newTask->TacheRadiation,
            "Radiation",
            10000,
            &lesDonnees,
            1,
            NULL);

    xTaskCreate(
            newTask->TacheGPS,
            "GPS",
            10000,
            &lesDonnees,
            1,
            NULL);
    xTaskCreate(
            newTask->TacheAfficher,
            "affiche",
            10000,
            &lesDonnees,
            2,
            NULL);




}

void loop() {

}

