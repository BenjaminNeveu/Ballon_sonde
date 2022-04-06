/* 
 * File:   sigfoxBallon.h
 * Author: bneveu
 *
 * Created on 18 mars 2022, 11:38
 */

#ifndef TRAMEBALLON_H
#define TRAMEBALLON_H

#include <sigfox.h>
#include <structures.h>

class SigfoxBallon : public Sigfox {
public:
    SigfoxBallon(uint8_t rxPin = 26, uint8_t txPin = 27, bool debugEn = true);

    void coderTrame(typeDonnees *lesDonnees);
    byte trame[12];

private:

    double arrondir(double val);
    float arrondir(float val);

};
#endif /* TRAMEBALLON_H */
