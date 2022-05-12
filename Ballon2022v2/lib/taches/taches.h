/*
 * File:   Taches.h
 * Author: bneveu
 *
 * Created on 31 mars 2022, 10:44
 */

#ifndef TACHES_H
#define TACHES_H

#include <Arduino.h>
//SigFox
#include <sigfox.h>
#include <sigfoxBallon.h>
//CarteSD
#include <SdCard.h>
//Structures
#include <structures.h>
//BME280
#include <Wire.h>
#include <BME280I2C.h>
//Radiation
#include <RadiationWatch.h>
//GPS
#include <TinyGPS.h>
#include <HardwareSerial.h>
//ServerWeb
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class Taches {
public:
    Taches();
    
    Taches(const Taches& orig);
    virtual ~Taches();
    
    static void tacheSigfox (void *parameter);
    static void tacheCarteSd (void *parameter);
    static void tacheBME280 (void *parameter);
    static void tacheRadiation (void *parameter);
    static void tacheGPS (void *parameter);
    static void tacheServeurWeb (void *parameter);
    
private:
    
};

#endif /* TACHES_H */

