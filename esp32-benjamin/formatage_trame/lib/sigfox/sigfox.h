
/* 
 * File:   sigfox.h
 * Author: bneveu
 *
 * Created on 18 mars 2022, 11:38
 */

#ifndef __SIGFOX__
#define __SIGFOX__


#include <Arduino.h>
#include <HardwareSerial.h>
#include <structures.h>

class Sigfox
{
  public:

    Sigfox(uint8_t rxPin, uint8_t txPin, bool debugEn);

    String tester(void);
    void begin(void);
    
    String   obtenirID(void);
    String   obtenirPAC(void);
    uint16_t obtenirTemp(void);

    bool envoyer(const void* data, uint8_t size);
    void coderTrame(typeDonnees *lesDonnees);
    byte trame[12];

  private:
    uint8_t rx, tx;
    HardwareSerial *serialSig;
    bool debug;
    String obtenirData(void);

};

#endif