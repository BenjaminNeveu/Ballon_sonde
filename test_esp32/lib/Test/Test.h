/* 
 * File:   Test.h
 * Author: cmaillard
 *
 * Created on 18 mars 2022, 10:45
 */

#ifndef TEST_H
#define TEST_H
#include <Arduino.h>
#define LED 22



class Test {
public:
    Test();
    Test(const Test& orig);
    virtual ~Test();
    void clignoter();
private:

};

#endif /* TEST_H */

