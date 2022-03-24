
/* 
 * File:   sigfox.cpp
 * Author: bneveu
 * 
 * Created on 18 mars 2022, 11:38
 */

#include <sigfox.h>

#include <structures.h>

//Message buffer
uint8_t msg[12];

Sigfox::Sigfox (uint8_t rxPin=26 , uint8_t txPin=27, bool debugEn=true) {
	serialSig = new HardwareSerial(2);
  rx = rxPin;
  tx = txPin;
  debug = debugEn;
}

// Lecture des datas reçu sur la liaison série HardwareSerial 
String Sigfox::obtenirData(void){
  String data = "";
  char output;

  while (!serialSig->available()){
    delay(100);
  }

  while(serialSig->available()){
    output = serialSig->read();
    if ((output != 0x0A) && (output != 0x0D)){       //0x0A Line feed | 0x0D Carriage return
      data += output;
      }
    delay(10);
  } 
  return data;  
}

// Initialise la liaison avec le composant Sigfox
void Sigfox::begin(){

	serialSig->begin(9600, SERIAL_8N1, rx, tx);

	if (debug) {
		//Serial.begin(9600);
		delay(100);
                tester();
                delay(100);
		obtenirID();
                obtenirTemp();
		delay(100);
		obtenirPAC();
	}
}

//Méthode pour obtenir l'identifiant Sigfox
String Sigfox::obtenirID(){
	serialSig->print("AT$I=10\r");
	String id = obtenirData();

	if(debug){
		Serial.print("ID: ");
		Serial.println(id);
	}

	return id;
}


//Méthode pour obtenir le PAC number
String Sigfox::obtenirPAC(){
	serialSig->print("AT$I=11\r");
	String pac = obtenirData();

	if(debug){
		Serial.print("PAC: ");
		Serial.println(pac);
	}

	return pac;
}

//Méthode pour obtenir la température du module
uint16_t Sigfox::obtenirTemp(void){
	serialSig->print("AT$T?\r");
	uint16_t tempVal = obtenirData().toInt();

	if(debug){
		Serial.print("Module temperature : ");
		Serial.println(tempVal);
	}

	return tempVal;
}

/** Méthode pour tester le composant 
 *  la commande AT renvoie OK
 */

 String Sigfox::tester(){
  serialSig->print("AT\r");
  String status = obtenirData();

  if(debug){
    Serial.print("Status : ");
    Serial.println(status);
  }

  return status;
}

/**
 * Méthode pour envoyer des data
 * data un tableau de 12 octets au maximum
 * size le nombre d'octet à envoyer
 * retourne vrai si le message a été envoyé avec success
 */
bool Sigfox::envoyer(const void* data, uint8_t size){   
  String status = "";
  char output;

  uint8_t* bytes = (uint8_t*)data;

  serialSig->print("AT$SF=");
  if(debug) { Serial.print("Byte : "); }
  for(uint8_t i= 0; i<size; ++i){
    serialSig->print(bytes[i] < 16 ? "0" : "");
    serialSig->print(bytes[i], HEX);
    
    if(debug){
      Serial.print(bytes[i] < 16 ? "0" : "");     
      Serial.print(bytes[i], HEX);
    }
  }
  if(debug) { Serial.println(" "); }
  serialSig->print("\r");

  String res = obtenirData();

  if(res.indexOf("OK") >= 0) {
    Serial.println("Message envoyé avec succès");
    return true;
  }
  
  if(debug){
    Serial.print("Status : ");
    Serial.println(res);
  }
  return false;
}

void Sigfox::coderTrame(typeDonnees *lesDonnees){
    
    int digit;
    int bitsignelongitude;
    int bitsignetemperature;
    int altitude = lesDonnees->position.altitude;
    double latitude = (lesDonnees->position.latitude - 40)*1000000 ;
    double longitude = lesDonnees->position.longitude * 1000000;
    float radiations = lesDonnees->DonneesCapteurs.cpm * 10;
    float pression = lesDonnees->DonneesCapteurs.pression * 10;
    float humidite = lesDonnees->DonneesCapteurs.humidite * 10;
    float temperature = lesDonnees->DonneesCapteurs.temperature * 10;
    
    //-------------------altitude--------------------
    
    Serial.printf("altitude : %d\n",(int)altitude);
    
    //-------------------latitude--------------------
     
    // recuperer le dernier digit
    digit = (int)latitude%10; //digit == 5
    if(digit >= 5){
        latitude = (latitude + 10)/10;
    }else{
        latitude =latitude/10;
    }
    
    Serial.printf("latitude : %d\n",(int)latitude);
    
    //-------------------longitude--------------------
    
    if((int)longitude < 0){
        longitude = longitude * -1;
        bitsignelongitude = 1;
    }else{
        bitsignelongitude = 0;
    }
    
    // recuperer le dernier digit
    digit = (int)longitude%10; //digit == 5
    if(digit >= 5){
        longitude = (longitude + 10)/10;
    }else{
        longitude =longitude/10;
    }
    
    Serial.printf("bit de signe : %d\n",bitsignelongitude);
    Serial.printf("longitude : %d\n",(int)longitude);
    
    //-------------------radiation--------------------
     
    // recuperer le dernier digit
    digit = (int)radiations%10; //digit == 5
    if(digit >= 5){
        radiations = (radiations + 10)/10;
    }else{
        radiations = radiations/10;
    }
    
    Serial.printf("radiation : %d\n",(int)radiations);
    
    //-------------------pression--------------------
     
    // recuperer le dernier digit
    digit = (int)pression%10; //digit == 5
    if(digit >= 5){
        pression = (pression + 10)/10;
    }else{
        pression = pression/10;
    }
    
    Serial.printf("pression : %d\n",(int)pression);
    
    //-------------------humidite--------------------
     
    // recuperer le dernier digit
    digit = (int)humidite%10; //digit == 5
    if(digit >= 5){
        humidite = (humidite + 10)/10;
    }else{
        humidite = humidite/10;
    }
    
    Serial.printf("humidite : %d\n",(int)humidite);
    
    //-------------------temperature--------------------
    
    if((int)temperature < 0){
        temperature = temperature * -1;
        bitsignetemperature = 1;
    }else{
        bitsignetemperature = 0;
    }
    
    // recuperer le dernier digit
    digit = (int)temperature%10; //digit == 5< 7
    if(digit >= 5){
        temperature = (temperature + 10)/10;
    }else{
        temperature =temperature/10;
    }
    
    Serial.printf("bit de signe : %d\n",bitsignetemperature);
    Serial.printf("temperature : %d\n",(int)temperature);
    
    Serial.printf("\n");
    
    //----------------coder-trame--------------------
    
    byte octet; //variable sur un octet (= 8bits)
    byte coteGauche;
    byte coteDroit;
    int i = 11; //indice du tableau (on commence à la fin du tableau)
    
    //trame[11]
    coteDroit = (int)temperature;
    coteGauche = (int)humidite << 7;
    octet= coteDroit | coteGauche;
    
    if(bitsignetemperature == 1){
        octet = octet | 0x40;
    }else{
        octet = octet & 0xBF;
    }
    trame[i] = octet;
    i--;
    
    //trame[10]
    coteDroit = (int)humidite >> 1;
    coteGauche = (int)pression << 6;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;
    
    //trame[9]
    octet = (int)pression >> 2;
    trame[i] = octet;
    i--;
    
    //trame[8]
    octet = (int)radiations;
    trame[i] = octet;
    i--;
    
    //trame[7]
    coteDroit = (int)radiations >> 8;
    coteGauche = (int)longitude << 7;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;
    
    //trame[6]
    octet = (int)longitude  >> 1;
    trame[i] = octet;
    i--;
    
    //trame[5]
    octet = (int)longitude  >> 9;
    trame[i] = octet;
    i--;
    
    //trame[4]
    coteDroit = (int)longitude  >> 17;
    coteGauche = (int)latitude << 4;
    octet= coteDroit | coteGauche;
    
    if(bitsignelongitude == 1){
        octet = octet | 0x08;
    }else{
        octet = octet & 0xF7;
    }
    trame[i] = octet;
    i--;

    //trame[3]
    octet = (int)latitude >> 4;
    trame[i] = octet;
    i--;
    
    //trame[2]
    octet = (int)latitude >> 12;
    trame[i] = octet;
    i--;
    
    //trame[1]
    coteDroit = (int)latitude >> 20;
    coteGauche = (int)altitude << 1;
    octet = coteDroit | coteGauche;
    trame[i] = octet;
    i--;
    
    //trame[0]
    octet = (int)altitude >> 7;
    trame[i] = octet;
    i--;
}