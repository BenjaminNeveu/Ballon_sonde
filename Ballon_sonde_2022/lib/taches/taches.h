/*
 * File:   Taches.h
 * Author: bneveu
 *
 * Created on 31 mars 2022, 10:44
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
    static void tacheAfficher(void *parameter);
    
private:
    
    
    
};

#endif /* TACHES_H */

