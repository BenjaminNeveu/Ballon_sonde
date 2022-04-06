/* 
 * File:   Test.cpp
 * Author: cmaillard
 * 
 * Created on 18 mars 2022, 10:45
 */

#include "Test.h"

Test::Test() {
     pinMode(LED, OUTPUT);
}

Test::Test(const Test& orig) {
}

Test::~Test() {
}

void Test::clignoter(){
    digitalWrite(LED, digitalRead(LED) ^1); 
    
}

