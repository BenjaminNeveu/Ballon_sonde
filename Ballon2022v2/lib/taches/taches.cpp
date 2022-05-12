/* 
 * File:   Taches.cpp
 * Author: bneveu
 * 
 * Created on 31 mars 2022, 10:44
 */

#define DEBUG false

// temps pour tacheCarteSD
#define TIME_CARTESD 30000
// temps pour tacheBME280
#define TIME_BME 30000
// temps pour tacheGPS
#define TIME_GPS 30000
// temps pour tacheRadiation
#define TIME_RADIATION 30000
// temps pour tacheAfficher
#define TIME_AFFICHER 30000

// temps pour tacheSigfox selon l'altitude
// avant décolage
#define TIME_SIG_DEPART 600000
// en dessous de 1200 m lors de la monter
#define TIME_SIG_DEBUT 300000
// au dessus de 1200 m
#define TIME_SIG_VOL 600000
// en dessous de 1200 m lors de la descente
#define TIME_SIG_FIN 180000

#include "taches.h"

// nom du point d'accès
const char *ssid = "Ballon_Sonde";
// mot de passe du point d'accès
// NULL = aucun mot de passe
const char *password = NULL;

SigfoxBallon *Sig;
SdCard CarteSD(14, 2, 15, 13, DEBUG);
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

    float altPrecedent = 0;
    float altActuel = 0;
    bool depart = true;
    int ticks;

    Sig = new SigfoxBallon(27, 26, DEBUG);
    Sig->begin();

    while (true) {

        if (DEBUG == true) {
            Serial.print("TacheSigfox \n");
        }
        xSemaphoreTake(mutex, portMAX_DELAY);
        //latitude 40.0 valeur du GPS non sync
        //Si le GPS n'est pas sync aucune trame est envoyé
        if (lesDonnees->latitude != 40.0) {
            altPrecedent = altActuel;
            altActuel = lesDonnees->altitude;
            Sig->coderTrame(lesDonnees);
            Sig->envoyer();
        }
        xSemaphoreGive(mutex);

        //vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_SIG_VOL));

        if (DEBUG == true) {
            Serial.print("altitude précédente : ");
            Serial.print(altPrecedent);
            Serial.print("\taltitude actuel : ");
            Serial.print(altActuel);
        }

        // si l'altitude passe au dessu de 200 m 
        if (altActuel >= 200) {
            depart = false;
        }
        if (depart == true) {
            // avant le depart 10 min
            ticks = TIME_SIG_DEPART;
            if (DEBUG == true) {
                Serial.println("\navant depart");
            }
        } else {
            if (altActuel < 1200) {
                if (altActuel > altPrecedent) {
                    if (DEBUG == true) {
                        Serial.println("\nmonter");
                    }
                    // envoie toutes les 5 minutes
                    ticks = TIME_SIG_DEBUT;
                } else {
                    if (DEBUG == true) {
                        Serial.println("\ndescente");
                    }
                    // envoie toutes les 3 minutes
                    ticks = TIME_SIG_FIN;
                }
                // sinon toutes les 10 minutes
            } else {
                Serial.println("\nvol");
                ticks = TIME_SIG_VOL;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ticks));

    }
}

void Taches::tacheCarteSd(void* parameter) {

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    CarteSD.begin();

    if (DEBUG == true) {
        // vérification si le fichier csv est déjà pésent
        // crétion du fichier si n'est pas présent
        if (SD.exists("/ballon.csv")) {
            Serial.println("fichier existe");
        } else {
            CarteSD.initFile("/ballon.csv", "datetime;altitude;longitude;latitude;radiation;pression;temperature;humidite\n");
            Serial.println("fichier cree");
        }
    } else {
        // vérification si le fichier csv n'est pas déjà pésent
        // crétion du fichier
        if (!SD.exists("/ballon.csv")) {
            CarteSD.initFile("/ballon.csv", "datetime;altitude;longitude;latitude;radiation;pression;temperature;humidite\n");
        }
    }


    while (true) {
        if (DEBUG == true) {
            Serial.print("TacheCarteSD\n");
        }
        xSemaphoreTake(mutex, portMAX_DELAY);
        // ajout de la chaine de caractere contenant les données, au fichier csv
        CarteSD.ajouter("/ballon.csv", CarteSD.creerChaine(lesDonnees));
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_CARTESD));
    }
}

void Taches::tacheBME280(void* parameter) {

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

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    while (!bme.begin()) {
        if (DEBUG == true) {
            Serial.println("erreur BME");
        }
        delay(10000);
    }

    while (true) {
        float temp(NAN), hum(NAN), pres(NAN);

        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_hPa);

        bme.read(pres, temp, hum, tempUnit, presUnit);

        if (DEBUG == true) {
            Serial.print("TacheBME\nTemp: ");
            Serial.print(temp);
            Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? 'C' : 'F'));
            Serial.print("\t\tHumidity: ");
            Serial.print(hum);
            Serial.print("% RH");
            Serial.print("\t\tPressure: ");
            Serial.print(pres);
            Serial.println("hPa");
        }

        // ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        lesDonnees->temperature = temp;
        lesDonnees->humidite = hum;
        lesDonnees->pression = pres;
        // fermeture du mutex
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_BME));
    }
}

void Taches::tacheServeurWeb(void* parameter) {

    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    //Activation du WiFi
    WiFi.softAP(ssid, password);

    //Initialisation SPIFFS
    if (DEBUG == true) {
        Serial.println("Inizializing FS...");
        if (SPIFFS.begin()) {
            Serial.println("SPIFFS mounted correctly.");
        } else {
            Serial.println("!An error occurred during SPIFFS mounting");
        }
        //affichage de l'adresse IP de l'ESP
        Serial.print("IP address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        SPIFFS.begin();
    }

    // initialisation des lien vers les fichier présent dans le fichier data
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

    // fonction recupererDonnee recupére les informations présentent dans la 
    // structure de données pour les afficher dans la page Web
    server.on("/recupererDonnee", HTTP_GET, [lesDonnees](AsyncWebServerRequest * request) {
        // ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        // réponse sous forme d'un fichier JSON
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
        // fermeture du mutex
        xSemaphoreGive(mutex);
        // envoie de la reponse
        request->send(response);
        if (DEBUG == true) {
            Serial.println("serveur Web envoi des données JSON");
        }
    });

    // fonction envoi trame Sigfox contenant les informations 
    // présentent dans la structure de données
    server.on("/envoiTrame", HTTP_GET, [lesDonnees](AsyncWebServerRequest * request) {
        // ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        // encodage de la trame
        Sig->coderTrame(lesDonnees);
        // fermeture du mutex
        xSemaphoreGive(mutex);
        // envoie de la trame
        Sig->envoyer();
        // envoie de la reponse
        request->send(200, "text/plain", "Success");
        if (DEBUG == true) {
            Serial.println("serveur Web envoi Trame");
        }
    });

    // fonction desactive le WiFi
    server.on("/desactiverWifi", HTTP_GET, [](AsyncWebServerRequest * request) {

        if (DEBUG == true) {
            Serial.println("WiFi off");
        }
        request->send(200, "text/plain", "WiFi off");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        WiFi.getSleep();
        server.end();
        if (DEBUG == true) {
            Serial.println("WiFi actif");
        }
        request->send(200, "text/plain", "WiFi actif");
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

    float lat, lon, alt = 0;
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
        if (newData) {
            gps.f_get_position(&lat, &lon, &age);
            gps.crack_datetime(&annee, &mois, &jour, &heure, &minute, &seconde, &centiemes, &age);
            alt = gps.f_altitude();

            if (DEBUG == true) {
                Serial.print("TacheGPS\nalt: ");
                Serial.print(alt);
                Serial.print("\t\tlon: ");
                Serial.print(lon);
                Serial.print("\t\tlat: ");
                Serial.print(lat);
                Serial.printf("\ndate: %d:%d:%d %d/%d/%d\n\n", heure, minute, seconde, jour, mois, annee);
            }

            // applique les valeur a la structure si le GPS est sync
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
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_GPS));
    }
}

void Taches::tacheRadiation(void* parameter) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;
    radiationWatch.setup();

    while (true) {
        radiationWatch.loop();
        //ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);
        lesDonnees->cpm = radiationWatch.cpm();
        if (DEBUG == true) {
            Serial.print("TacheRadiation\ncpm: ");
            Serial.println(lesDonnees->cpm);
        }
        //fermeture du mutex
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_RADIATION));
    }
}

void Taches::tacheAfficher(void* parameter) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    typeDonnees *lesDonnees = (typeDonnees *) parameter;

    while (true) {
        //ouverture du mutex
        xSemaphoreTake(mutex, portMAX_DELAY);

        Serial.print("\n\ntemperature :");
        Serial.print(lesDonnees->temperature);
        Serial.print("\t\thumidité :");
        Serial.print(lesDonnees->humidite);
        Serial.print("\t\tpression :");
        Serial.print(lesDonnees->pression);
        Serial.print("\naltitude : ");
        Serial.print(lesDonnees->altitude);
        Serial.print("\t\tlongitude :");
        Serial.print(lesDonnees->longitude);
        Serial.print("\t\tlatitude :");
        Serial.print(lesDonnees->latitude);
        Serial.print("\nannee :");
        Serial.print(lesDonnees->annee);
        Serial.print("\t\tmois :");
        Serial.print(lesDonnees->mois);
        Serial.print("\t\tjour :");
        Serial.print(lesDonnees->jour);
        Serial.print("\nheure :");
        Serial.print(lesDonnees->heure);
        Serial.print("\t\tminutes :");
        Serial.print(lesDonnees->minute);
        Serial.print("\t\tseconde :");
        Serial.print(lesDonnees->seconde);
        Serial.print("\nCPM :");
        Serial.println(lesDonnees->cpm);
        //fermeture du mutex
        xSemaphoreGive(mutex);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIME_AFFICHER));
    }
}