
/* 
 * File:   main.cpp
 * Author: bneveu
 *
 * Created on 22 mars 2022, 11:38
 */

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <sigfox.h>

/*
 * 
 */

typeDonnees lesDonnees;
const char *ssid = "Ballon_Sonde";
const char *password = NULL;

Sigfox Sig(27, 26, true);
AsyncWebServer server(80);

void setup() {
    //liaison serie
    Serial.begin(115200);

    Sig.begin();

    lesDonnees.DonneesCapteurs.cpm = 100.81;
    lesDonnees.DonneesCapteurs.humidite = 85.60;
    lesDonnees.DonneesCapteurs.pression = 1000.14;
    lesDonnees.DonneesCapteurs.temperature = -15.25;
    lesDonnees.position.altitude = 28056.25;
    lesDonnees.position.latitude = 47.089909;
    lesDonnees.position.longitude = -1.270549;

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

    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();

    Serial.println();
    Serial.println("===== File system info =====");

    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");

    Serial.println();
    // Open dir folder
    File dir = SPIFFS.open("/");
    // List file at root
    listFilesInDir(dir, 1);

    Serial.println();
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


    server.on("/recupererDonnee", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(1024);
        json["longitude"] = lesDonnees.position.longitude;
        json["latitude"] = lesDonnees.position.latitude;
        json["altitude"] = lesDonnees.position.altitude;
        json["radiation"] = lesDonnees.DonneesCapteurs.cpm;
        json["temperature"] = lesDonnees.DonneesCapteurs.temperature;
        json["humidite"] = lesDonnees.DonneesCapteurs.humidite;
        json["pression"] = lesDonnees.DonneesCapteurs.pression;
        serializeJson(json, *response);
        request->send(response);
    });

    server.on("/envoiTrame", HTTP_GET, [](AsyncWebServerRequest * request) {
        Sig.coderTrame(& lesDonnees)
        Sig.envoyer(Sig.trame, sizeof (Sig.trame));
        request->send(200);
    });

    server.begin();
}

void loop() {
    delay(200);
}

void listFilesInDir(File dir, int numTabs) {
    while (true) {

        File entry = dir.openNextFile();
        if (!entry) {
            // no more files in the folder
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            listFilesInDir(entry, numTabs + 1);
        } else {
            // display zise for file, nothing for directory
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}