/* 
 * File:   structures.h
 * Author: bneveu
 *
 * Created on 18 mars 2022, 13:35
 */

#include <Arduino.h>

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    byte heure;
    byte minute;
    byte seconde;
    byte jour;
    byte mois;
    unsigned int annee;
    float cpm;
    float pression;
    float humidite;
    float temperature;
    float altitude;
    double latitude;
    double longitude;
} typeDonnees;

#endif /* STRUCTURES_H */

