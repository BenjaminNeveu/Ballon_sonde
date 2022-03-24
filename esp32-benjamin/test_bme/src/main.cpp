/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 18 mars 2022, 12:37
 */

#include <Arduino.h>
#include <Wire.h>
#include <BME280.h>
#include <BME280I2C.h>
#define SERIAL_BAUD 115200

BME280I2C::Settings parametrage(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x77 // I2C address pour BME 280 Adafruit.
);

BME280I2C bme(parametrage);    

void printBME280Data(Stream* client);


void setup()
{
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait

  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}


void loop()
{
   printBME280Data(&Serial);
   delay(500);
   
}

void printBME280Data( Stream* client)
{
   float temp(NAN), hum(NAN), pres(NAN), lux(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print(" °C\n");
   client->print("Humidité: ");
   client->print(hum);
   client->print("%\n");
   client->print("Pression: ");
   client->print(pres/100);
   client->print(" hPa\n");
   client->print("\n");

   delay(1000);
}