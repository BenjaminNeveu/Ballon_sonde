/**
 * @file structures.h
 * @brief Déclaration de la classe structures
 * @version 1.0
 * @author Sofiane SABRI
 * @date 20/05/2021
 */
#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    byte seconde;
    byte minute;
    byte heure;
} typeHeure;

typedef struct {
    byte jour;
    byte mois;
    unsigned int annee;
} typeDate;

typedef struct {
    float temperature;
    float humidite;
    float pression;
    float cpm;

} typeDonneesCapteurs;

typedef struct {
    float altitude;
    float latitude;
    float longitude;
} typePosition;

typedef struct {
    typePosition position;
    typeHeure heures;
    typeDate date;
    typeDonneesCapteurs DonneesCapteurs;
} typeDonnees;

    


#endif /* STRUCTURES_H */

