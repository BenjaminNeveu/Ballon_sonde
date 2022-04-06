/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newmain.cpp
 * Author: cmaillard
 *
 * Created on 30 mars 2022, 10:20
 */

#include <Arduino.h>
#include <Taches.h>




typeDonnees lesDonnees;



void setup() {


    Serial.begin(115200);
    Wire.begin(21, 22); // redefition des bornes SDA et SCL pour la carte ballon



    Taches *newTask = new Taches();


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




}

void loop() {




}
