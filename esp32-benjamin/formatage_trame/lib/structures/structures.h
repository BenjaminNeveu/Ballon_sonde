/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   structures.h
 * Author: bneveu
 *
 * Created on 18 mars 2022, 13:35
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    uint8_t seconde;
    uint8_t minute;
    uint8_t heure;
} typeHeure;

typedef struct {
    uint8_t jour;
    uint8_t mois;
    int annee;
} typeDate;

typedef struct {
    float cpm;
    float pression;
    float humidite;
    float temperature;
} typeDonneesCapteurs;

typedef struct {
    int altitude;
    double latitude;
    double longitude;
} typePosition;

typedef struct {
    typePosition position;
    typeHeure heures;
    typeDate date;
    typeDonneesCapteurs DonneesCapteurs;
} typeDonnees;

#endif /* STRUCTURES_H */

