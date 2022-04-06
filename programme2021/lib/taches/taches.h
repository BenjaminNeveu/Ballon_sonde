/**
 * @file taches.h
 * @brief Déclaration de la classe Taches
 * @version 1.0
 * @author Sofiane SABRI
 * @date 20/05/2021
 */
#ifndef TACHES_H
#define TACHES_H
#define SERIAL_BAUD 115200
#define LED 22
#define SERIALGPS 1



#include "structures.h"
#include <Arduino.h>
#include <Wire.h>
#include <BME280I2C.h>
#include <RadiationWatch.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <string>
#include <HardwareSerial.h>
#include "sigfox.h"
class Taches {
public:
    Taches();
    //void ObtenirTaches(void * parameter);
   static void tacheBME280(void * parameter);
   static void tacheRadiations(void* parameter);
   static void tacheGPS(void* parameter);
   static void tacheAffichage(void* parameter);
   static void tacheSigfox(void* parameter);
   static void tacheCarteSD(void* parameter);
private:
   
  
};


#endif /* TACHES_H */

