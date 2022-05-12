/* 
 * File:   Taches.cpp
 * Author: cmaillard
 * 
 * Created on 30 mars 2022, 10:19
 */

#include "Taches.h"


SemaphoreHandle_t mutex;
RadiationWatch radiationWatch(32, 33);
TinyGPS gps;
HardwareSerial ss(2);

Taches::Taches() {
    mutex = xSemaphoreCreateMutex();
}

Taches::Taches(const Taches& orig) {
}

Taches::~Taches() {
}

/*
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
 */
void Taches::TacheBME(void *Parameters) {

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
    typeDonnees *donneesBme = (typeDonnees *) Parameters;
    BME280I2C bme(setBme);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (!bme.begin()) {
        Serial.println("erreur BME");
        delay(1000);
    }



    for (;;) {
        float temp(NAN), hum(NAN), pres(NAN);

        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_hPa);

        bme.read(pres, temp, hum, tempUnit, presUnit);
        //ouverture du mutex

        xSemaphoreTake(mutex, portMAX_DELAY);

       
        donneesBme->humidite = hum;
        donneesBme->pression = pres;
        donneesBme->temperature = temp;
        //fermeture du mutex
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}

void Taches::TacheRadiation(void *Parameters) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *donneesRadiation = (typeDonnees *) Parameters;
    radiationWatch.setup();
    // Register the callbacks.
    //radiationWatch.registerRadiationCallback(&onRadiation);
    //radiationWatch.registerNoiseCallback(&onNoise);


    for (;;) {
        radiationWatch.loop();
        //ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        donneesRadiation->cpm = radiationWatch.cpm();
        //fermeture du mutex
        xSemaphoreGive(mutex);


        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));

    }

}

void Taches::TacheGPS(void* Parameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *donneesGps = (typeDonnees *) Parameters;
    ss.begin(4800, SERIAL_8N1, 16, 17);
    float lat, lon, alt;
    ;

    unsigned long age;
    //HEURE
    byte centiemes, seconde, minute, heure;
    //DATE
    byte jour, mois;
    int annee;
    bool newData = false;

    for (;;) {
        // Pendant une seconde, nous analysons les données GPS et rapportons quelques valeurs clés
        for (unsigned long start = millis(); millis() - start < 1000;) {
            while (ss.available()) {
                char c = ss.read();
                // Serial.write(c); 
                if (gps.encode(c)) // si des caratères sont reçus alors newdata = true
                    newData = true;
            }
        }


        if (newData) {
            gps.f_get_position(&lat, &lon, &age);

            gps.crack_datetime(&annee, &mois, &jour, &heure, &minute, &seconde, &centiemes, &age);
            alt = gps.f_altitude();

            //ouverture du mutex
            xSemaphoreTake(mutex, portMAX_DELAY);

            donneesGps->altitude = alt;
            donneesGps->longitude = lon;
            donneesGps->latitude = lat;
            donneesGps->annee = annee;
            donneesGps->mois = mois;
            donneesGps->jour = jour;
            donneesGps->heure = heure;
            donneesGps->minute = minute;
            donneesGps->seconde = seconde;

            //fermeture du mutex
            xSemaphoreGive(mutex);


        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}

void Taches::TacheAfficher(void* Parameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) Parameters;


    for (;;) {



        //ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);


        Serial.println("");
        Serial.print("temperature :");
        Serial.print(lesDonnees->temperature);
        Serial.print("\t\t humidité :");
        Serial.print(lesDonnees->humidite);
        Serial.print("\t\t pression :");
        Serial.println(lesDonnees->pression);

        /*
        Serial.print("altitude : ");
        Serial.print(lesDonnees->altitude);
        Serial.print("\t\t longitude :");
        Serial.print(lesDonnees->longitude);
        Serial.print(" \t\tlatitude :");
        Serial.println(lesDonnees->latitude);

        Serial.print("annee :");
        Serial.print(lesDonnees->annee);
        Serial.print("\t\t mois :");
        Serial.print(lesDonnees->mois);
        Serial.print("\t\t jour :");
        Serial.println(lesDonnees->jour);

        Serial.print("heure :");
        Serial.print(lesDonnees->heure);
        Serial.print("\t \t minutes :");
        Serial.print(lesDonnees->minute);
        Serial.print("\t \t seconde :");
        Serial.println(lesDonnees->seconde);

        Serial.print("CPM :");
        Serial.print(lesDonnees->cpm);
         */


        //fermeture du mutex
        xSemaphoreGive(mutex);



        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}