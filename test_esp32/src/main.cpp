/* 
 * File:   main.cpp
 * Author: cmaillard
 *
 * Created on 18 mars 2022, 10:40
 */

#include <Arduino.h>
#include <Test.h>
#include <BME280I2C.h>          // Capteur Pression humidité température
#include <TinyGPS.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <RadiationWatch.h>

typedef struct {
    char date[8];
    char time[6];
} typeDateTime;

typedef struct {
    float cpm;
    float pression;
    float humidite;
    float temperature;
} typeDonneesCapteurs;

typedef struct {
    unsigned int altitude;
    double latitude;
    double longitude;
} typePosition;

typedef struct {
    typePosition position;
    typeDateTime dateTime;
    typeDonneesCapteurs donneesCapteurs;
} typeDonnees;

typeDonnees lesDonnees;

BME280I2C::Settings setBme(
        BME280::OSR_X1,
        BME280::OSR_X1,
        BME280::OSR_X1,
        BME280::Mode_Forced,
        BME280::StandbyTime_1000ms,
        BME280::Filter_Off,
        BME280::SpiEnable_False,
        BME280I2C::I2CAddr_0x77 // I2C address pour BME 280.
        );

BME280I2C bme(setBme);
Test leTest;


RadiationWatch radiationWatch(32, 33);

void onRadiation() {

    Serial.println("Un rayon gamma est apparu");
    Serial.print(radiationWatch.uSvh());
    Serial.print(" uSv/h +/- ");
    Serial.println(radiationWatch.uSvhError());
    Serial.print(" CPM : ");
    Serial.println(radiationWatch.cpm());
}

void onNoise() {
    Serial.println("Argh, bruit, SVP arreter de bouger");
}

void donneeBME(void *pvParameters) {
    BME280I2C(setBme);

    while (!bme.begin()) {
        Serial.println("erreur");
        delay(1000);
    }
    
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
   
    for (;;) {
        float temp(NAN), hum(NAN), pres(NAN);

        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_hPa);

        bme.read(pres, temp, hum, tempUnit, presUnit);

        Serial.print("Temp: ");
        Serial.print(temp);
        Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
        Serial.print("\t\tHumidity: ");
        Serial.print(hum);
        Serial.print("% RH");
        Serial.print("\t\tPressure: ");
        Serial.print(pres);
        Serial.println("hPa");
        
        
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

void radiation(void *pvParameters) {

    radiationWatch.setup();
    // Register the callbacks.
    radiationWatch.registerRadiationCallback(&onRadiation);
    
    radiationWatch.registerNoiseCallback(&onNoise);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    for (;;) {
        radiationWatch.loop();
        
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
        
    }
    
}

void setup() {


    Serial.begin(115200);
    Wire.begin(21, 22); // redefition des bornes SDA et SCL pour la carte ballon




    Serial.println("Setup done");
    //xTaskCreate(donneeBME, "donneeBME", 10000, NULL, 1, NULL);
    xTaskCreate(radiation, "radiation", 10000, NULL, 1, NULL);

}

void loop() {
    Serial.printf("Task loop() %d\n", xPortGetCoreID());
    delay(60000);
}



