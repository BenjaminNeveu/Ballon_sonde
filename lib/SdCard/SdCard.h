/* 
 * File:   SdCard.h
 * Author: bneveu
 *
 * Created on 28 mars 2022, 08:56
 */

#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <string>

class SdCard {
public:
    SdCard(uint8_t sckPin = 14, uint8_t misoPin = 2, uint8_t mosiPin = 15, uint8_t csPin = 13);

    SdCard(const SdCard& orig);
    virtual ~SdCard();
    
    bool begin();
    
    void initFile(const char* path, const char* message);
    
    void creerChaine (typeDonnees *lesDonnees);
    
    void ajouter(const char* path, String chaine);
    
    String chaine;
    
private:
    
    String formatDateHeure(byte val);
    
    uint8_t cs;
};

#endif /* SDCARD_H */
