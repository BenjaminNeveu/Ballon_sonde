/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Taches.h
 * Author: cmaillard
 *
 * Created on 30 mars 2022, 10:19
 */

#ifndef TACHES_H
#define TACHES_H

#include <Arduino.h>
#include <BME280.h> 
#include <BME280I2C.h>          // Capteur Pression humidité température
#include <TinyGPS.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <RadiationWatch.h>
#include <Structure.h>

 

class Taches {
public:
    Taches();
    Taches(const Taches& orig);
    virtual ~Taches();



    static void TacheBME(void *Parameters);
    static void TacheRadiation(void *Parameters);
    static void TacheGPS(void *Parameters);

private:

};

#endif /* TACHES_H */

