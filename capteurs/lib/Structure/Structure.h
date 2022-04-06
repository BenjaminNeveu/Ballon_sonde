/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Structure.h
 * Author: cmaillard
 *
 * Created on 31 mars 2022, 14:51
 */

#ifndef STRUCTURE_H
#define STRUCTURE_H

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
    unsigned int altitude;
    double latitude;
    double longitude;
} typeDonnees;


#endif /* STRUCTURE_H */

