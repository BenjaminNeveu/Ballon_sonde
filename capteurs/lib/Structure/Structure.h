/**
    @file   Structure.h
    @brief Déclaration de la classe structure
    @version 1.0
    @author cmaillard
    @date 30/03/2022
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

