/**
 @file   Taches.h
 @brief Déclaration de la classe Taches
 @version 1.0
 @author cmaillard
 @date 30/03/2022
 */

#ifndef TACHES_H
#define TACHES_H

#include <Arduino.h>            // framework arduino
#include <BME280.h>             // fonctionnement du bme
#include <BME280I2C.h>          // Capteur Pression humidité température
#include <TinyGPS.h>            //capteur gps et date
#include <HardwareSerial.h>     // liaison serie
#include <Wire.h>               // bornes de la carte 
#include <RadiationWatch.h>     // capteur radiation
#include <Structure.h>          // struture pour la sauvegarde des données

 

class Taches {
public:
    Taches();
    Taches(const Taches& orig);
    virtual ~Taches();



    static void TacheBME(void *parameters);
    static void TacheRadiation(void *parameters);
    static void TacheGPS(void *parameters);
    static void TacheAfficher(void* parameters);

private:

};

#endif /* TACHES_H */

