/* 
 * File:   Taches.cpp
 * Author: bneveu
 * 
 * Created on 31 mars 2022, 10:44
 */

#include "taches.h"

const char *ssid = "Ballon_Sonde";
const char *password = NULL;

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

SigfoxBallon *Sig;
SdCard CarteSD(14, 2, 15, 13);
RadiationWatch radiationWatch(32, 33);
TinyGPS gps;
HardwareSerial ss(2);

AsyncWebServer server(80);

SemaphoreHandle_t mutex;

Taches::Taches() {
    mutex = xSemaphoreCreateMutex();
}

Taches::Taches(const Taches& orig) {
}

Taches::~Taches() {
}

void Taches::tacheSigfox(void* parameter) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    float altPrecedent;
    float altActuel;
    int ticks;

    Sig = new SigfoxBallon(27, 26, true);
    Sig->begin();

    while (true) {
        
        Serial.print("TacheSigfox \n");
        xSemaphoreTake(mutex, portMAX_DELAY);
        if (lesDonnees->latitude != 40.0) {
            Sig->coderTrame(lesDonnees);
            Sig->envoyer();
        }
        xSemaphoreGive(mutex);
        
        if (lesDonnees->altitude > 1020) {
            ticks = 600000;
            Serial.println("10 min");
        } else {
            ticks = 240000;
            Serial.println("4 min");
        }
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ticks));
    }
}

void Taches::tacheCarteSd(void* parameter) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;
    
    CarteSD.begin();
    
    if(SD.exists("/ballon.csv")){
        Serial.println("fichier existe");
    }else{
        CarteSD.initFile("/ballon.csv", "datetime;altitude;longitude;latitude;radiation;pression;temperature;humidite\n");
        Serial.println("fichier cree");
    }
    
    while (true) {
        Serial.print("TacheCarteSD\n");
        xSemaphoreTake(mutex, portMAX_DELAY);
        CarteSD.ajouter("/ballon.csv", CarteSD.creerChaine(lesDonnees));
        xSemaphoreGive(mutex);
        
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(60000));
    }
}

void Taches::tacheBME280(void* parameter) {

    while (!bme.begin()) {
        Serial.println("erreur BME");
        delay(1000);
    }

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    while (true) {

        float temp(NAN), hum(NAN), pres(NAN);

        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_hPa);

        bme.read(pres, temp, hum, tempUnit, presUnit);

        Serial.print("TacheBME\nTemp: ");
        Serial.print(temp);
        Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
        Serial.print("\t\tHumidity: ");
        Serial.print(hum);
        Serial.print("% RH");
        Serial.print("\t\tPressure: ");
        Serial.print(pres);
        Serial.println("hPa");

        xSemaphoreTake(mutex, portMAX_DELAY);

        lesDonnees->temperature = temp;
        lesDonnees->humidite = hum;
        lesDonnees->pression = pres;

        xSemaphoreGive(mutex);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(30000));
    }
}

void Taches::tacheServeurWeb(void* parameter) {

    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    //Activation du WiFi
    WiFi.begin();
    WiFi.softAP(ssid, password);

    //Initialisation SPIFFS
    Serial.println(F("Inizializing FS..."));
    if (SPIFFS.begin()) {
        Serial.println(F("SPIFFS mounted correctly."));
    } else {
        Serial.println(F("!An error occurred during SPIFFS mounting"));
    }

    //address IP ESP
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/fonctions.js", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(SPIFFS, "/fonctions.js", "text/javascript");
    });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(SPIFFS, "/styles.css", "text/css");
    });

    server.on("/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(SPIFFS, "/jquery-3.6.0.min.js", "text/javascript");
    });

    server.on("/recupererDonnee", HTTP_GET, [lesDonnees](AsyncWebServerRequest * request) {

        xSemaphoreTake(mutex, portMAX_DELAY);

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(1024);
        json["longitude"] = lesDonnees->longitude;
        json["latitude"] = lesDonnees->latitude;
        json["altitude"] = String(lesDonnees->altitude, 0);
        json["radiation"] = String(lesDonnees->cpm, 2);
        json["temperature"] = String(lesDonnees->temperature, 2);
        json["humidite"] = String(lesDonnees->humidite, 2);
        json["pression"] = String(lesDonnees->pression, 2);
        serializeJson(json, *response);
        request->send(response);
        Serial.println("serveur Web envoi des données JSON");

        xSemaphoreGive(mutex);

    });

    server.on("/envoiTrame", HTTP_GET, [lesDonnees](AsyncWebServerRequest * request) {

        xSemaphoreTake(mutex, portMAX_DELAY);
        Sig->coderTrame(lesDonnees);
        xSemaphoreGive(mutex);
        Sig->envoyer();
        Serial.println("serveur Web envoi Trame");
        request->send(200, "text/plain", "Success");

    });

    server.on("/desactiverWifi", HTTP_GET, [](AsyncWebServerRequest * request) {

        Serial.println("WiFi off");
        request->send(200, "text/plain", "WiFi off");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        WiFi.getSleep();
        server.end();
        Serial.println("WiFi erreur");
        request->send(200, "text/plain", "WiFi erreur");

    });

    server.begin();

    while (true) {
        vTaskDelay(1); //indispensable car sinon guru ?!
        delay(100);
    }
}

void Taches::tacheGPS(void* parameter) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;
    ss.begin(4800, SERIAL_8N1, 16, 17);
    float lat, lon, alt;

    unsigned long age;
    //HEURE
    byte centiemes, seconde, minute, heure;
    //DATE
    byte jour, mois;
    int annee;
    bool newData = false;

    while (true) {
        // Pendant une seconde, nous analysons les données GPS et rapportons quelques valeurs clés
        for (unsigned long start = millis(); millis() - start < 1000;) {
            while (ss.available()) {
                char c = ss.read();
                // Serial.write(c); 
                if (gps.encode(c)) // si des caratères sont reçus alors newdata = true
                    newData = true;
            }
        }

        gps.f_get_position(&lat, &lon, &age);
        gps.crack_datetime(&annee, &mois, &jour, &heure, &minute, &seconde, &centiemes, &age);
        alt = gps.f_altitude();

        if (alt != 1000000.00) {
            //ouverture du mutex
            xSemaphoreTake(mutex, portMAX_DELAY);
            lesDonnees->altitude = alt;
            lesDonnees->longitude = lon;
            lesDonnees->latitude = lat;
            lesDonnees->annee = annee;
            lesDonnees->mois = mois;
            lesDonnees->jour = jour;
            lesDonnees->heure = heure;
            lesDonnees->minute = minute;
            lesDonnees->seconde = seconde;
            //fermeture du mutex
            xSemaphoreGive(mutex);
        }

        Serial.print("TacheGPS\nalt: ");
        Serial.print(alt);
        Serial.print("\t\tlon: ");
        Serial.print(lon);
        Serial.print("\t\tlat: ");
        Serial.print(lat);
        Serial.printf("\ndate: %d:%d:%d %d/%d/%d\n\n", heure, minute, seconde, jour, mois, annee);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(30000));

    }
}

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

void Taches::tacheRadiation(void* parameter) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *donneesRadiation = (typeDonnees *) parameter;
    radiationWatch.setup();
    //Register the callbacks.
    //radiationWatch.registerRadiationCallback(&onRadiation);
    //radiationWatch.registerNoiseCallback(&onNoise);

    while (true) {
        radiationWatch.loop();
        //ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        donneesRadiation->cpm = radiationWatch.cpm();
        Serial.print("TacheRadiation\ncpm: ");
        Serial.println(donneesRadiation->cpm);
        //fermeture du mutex
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(30000));
    }
}
