# Ballon Sonde 2022 - BTS SNIR - Lycée Touchard Washington - Le Mans
**Important**

Ce Github concerne que la partie ballon
Le programme a été compilé sur un ESP32 TTGO-TI v1.3
Le projet a été réalisé sur l'IDE Netbeans avec PlatformIO d'installé (**Utilisation de l'outils SPIFFS**)

<hr>

Bibliothèque à installer :

- TinyGPS ``pio lib -g install 416``

- BME280 ``pio lib -g install 901``

- radiationwatch ``pio lib -g install 1523``

- ESPAsyncWebServer / ESPAsyncTCP / AsyncTCP ``pio lib -g install 306``

- ArduinoJson ``pio lib -g install 64``

<hr>

***Sommaire***
 1. [Présentation du projet](https://github.com/BenjaminNeveu/Ballon_sonde#pr%C3%A9sentation_du_projet)
 2. [Synoptique](https://github.com/BenjaminNeveu/Ballon_sonde#synoptique)
 3. [Cas d'utilisation](https://github.com/BenjaminNeveu/Ballon_sonde#cas_dutilisation)
    - [Émettre trame SigFox]()
      - [Présentation de Sigfox]()
      - [Format de la trame employé]()
        - [Altitude]() 
        - [Coordonnées]()
        - [Pression]()
        - [Humidité]()
        - [Température]()
        - [Radiation]()
      - [Diagramme de classe]() 
    - [Sauvegarder mesures et positions]()
      - [Classe utilisé]() 
    - [Afficher page web admin]()
      - [Interface]() 
4. [Amélioration a apporter]()
    - [Sauvegarder mesures et position]()
    - [Émettre trame SigFox]()
5. [Informations Complémentaires]()

## Présentation du projet


Le but principal du projet était de répondre à la problématique suivante . 
- Dans quelle mesure l’atmosphère nous protège-t-elle des radiations cosmiques. 

Ainsi, la récupération des données c'est réalisée à l’aide de capteurs présents dans la nacelle d'un ballon sonde stratosphérique.

<table>
<td>Les  données  récupérées : <br>
 - la température (°C)<br>
 - l’humidité (%)<br>
 - la pression (hPa)<br>
 - les rayonnements gamma et bêta (mSv/h)<br>
 - la position (en degrés décimaux)<br>
 - l’altitude du ballon (m)</td>
<td>
<img src="/img/img_ballon.png" title="Ballon Stratosphérique">
</td>
</table>

<table>
<td><img src="/img/F4KMN.png" title="Logo F4KMN"></td>
<td>
Ces informations ont été traitées et mis sur un site web pour avoir un suivi durant et après le voyage du ballon.
<br><br>L'année précédente la technologie SigFox avait été utilisée avec succès pour la transmission des données en provenance de la nacelle. C'est donc pour cela que nous avons réutilisé cette technologie.<br><br>Cependant le lycée souhaita également associer l’opérateur SigFox avec le système de communication APRS (Système automatique de notification des paquets) afin de remédier à tout problème de communication. On souhaitait pouvoir visualiser la position du ballon par rapport au véhicule, pour permettre la récupération de la nacelle, ceci se fera à l’aide de l'APRS, une technologie radio amateur, de plus le lycée Touchard Washington aux Mans possède son propre radio club, son indicatif est F4KMN et appartient au REF (Réseau des Émetteurs Français).
</td>
</table>

<table>
	<td>
D’autre part, pour réaliser l’envoi du ballon, des autorisations sont nécessaires. Le projet de lancé un ballon-sonde en haute atmosphère a donc put être réalisable grâce aux partenariat avec Planète Science, association partenaire du CNES (Centre National d’Études Spatiales).
		<br><br>
Il met à la disposition des établissements scolaires (lycée, collèges, primaires) le matériel, tel que, le ballon, le parachute, le cordage, l’hélium  et ainsi que le cahier des charges que l’on doit respecter pour permettre le vol de celui-ci (poids, taille, ...), tous cela  pour favorisé la pratique des activités scientifiques, techniques et d’éducation à l’environnement auprès des jeunes et de leur famille dans une perspective éducative.
	</td>
	<td> 
		<img src="/img/Logo_planetesciences.png" title="Logo de Planète Science"> 
		<br><br>
		<img src="/img/Logo_CNES.png" title="Ballon Stratosphérique"> 
	</td>
<table>

## Synoptique

<img src="/img/Synoptique_global.png" title="Synoptique Global">

<img src="/img/Synoptique_ballon.png" title="Synoptique du Ballon">

<hr>

## Cas d'utilisation

<img src="/img/cas_dutilisation.png" title="Cas d'utilisation dans le ballon">

<hr>

### Émettre trame SigFox

>#### Présentation de Sigfox

SigFox est une technologie de communication française créée en septembre 2009 par Christophe Fourtet et Ludovic Le Moan. SigFox est spécialisé dans le M2M (machine to machine = communication entre machines). La technologie SigFox permet aux petits objets de pouvoir communiquer étant pas assez puissants pour communiquer avec les grands réseaux mobiles. SigFox a donc innové dans le domaine de la connectivité bas débit pour compléter les solutions à large bande passante. SigFox fournit des services de communication pour révéler le véritable potentiel de l’internet des objets (IoT).

SigFox utilise une plage de fréquences de 192 kHz situés à environ 868 MHz pour échanger des messages par liaison radio, chaque message occupe 100 Hz.

Pour répondre aux contraintes de coûts et d’autonomie des objets isolés fonctionnant sur des batteries, SigFox a conçu un protocole de communication pour les petits messages dont la taille est comprise entre 0 et 12 octets. Une charge utile de 12 octets suffit à transférer les données produites par un capteur, le statut d’un événement tel qu’une alerte, des coordonnées GPS voire des données d’application.

La puissance du signal est de 20 mW ce qui permet d’avoir une consommation réduite de la batterie. 

La portée d’un signal est de 25 km, cette distance est théorique, car elle dépend de différents facteurs donc les structures environnantes présentes, dans notre cas cette distance n’est pas une limite du fait de l’altitude du ballon. 

>#### Format de la trame employé

Contraintes : 
 - 12 octets
 - 140 messages par jour

La trame de 12 octets soit un total de 96 bits sera composé de l’altitude, la longitude, la latitude, la radiation, la pression, l’humidité et de la température.

Codage de la trame :
- Altitude : 15 bits
- Longitude : 20 bits + 1 bits de signe
- Latitude : 21 bits
- Pression : 10 bits
- Humidité : 7 bits
- Température : 6 bits +  bits de signe
- Radiation : 15 bits

##### Altitude 
L’année précédente le ballon a atteint 27252 m et peut atteindre environ les 30 km, j’ai donc décidé de prendre 15 bits, soit une valeur maximale de 32767. 

##### Coordonnées

Pour la position j’ai cherché la latitude et la longitude, minimal et maximal.

<img src="/img/Carte_France.png" title="Carte de France">

````
Lycée Touchard Washington,Le Mans
coordonnées : 
 - Latitude    47.99516296386719
 - Longitude   0.20449140667915344
````
Pour commencer j'ai d'abord essayé de déterminer la précision à avoir pour éliminer un maximum de chiffres, en sachant que 1° est égale a environ 111 km, j'ai calculé le nombre de digits après la virgule nécessaire pour avoir une précision d'environ 1 m, en vérifiant mes résultat sur une carte, j'en ai conclu qu'il était nécessaire d'avoir 5 digits après la virgule pour la précision souhaité.

###### Latitude
 
Pour économiser des bits j’ai décidé de faire un offset de 40, car la latitude minimale de la France est de environ 42°, ce qui me donne plage entre 2 et 11.

###### Longitude

Pour la longitude j’ai utilisé 20 bits pour la valeur et 1 bit de signe, car à l'ouest du méridien de Greenwich la longitude est négative.

Une toute les ajustement réalisé il ne reste plus qu'à multiplier la longitude et la latitude ``100000`` pour décaler la virgule et avoir la position souhaité. Par la suite toute les modifications apporté avant l'envoi seront inverser une fois réceptionnées pour quelle soit compréhensible et visible sur une carte.

##### Pression 
La pression atmosphérique est en moyenne de 1013 hPa au niveau de la mer, l’année précédente la pression a atteint 1015 au sol et sachant que la pression diminue avec l'altitude et il n'est pas nécessaire d'avoir une valeur bien plus grande, j’ai donc pris 10 bits ce qui correspond à une valeur max de 1023.

##### Humidité
Pour l’humidité, l’année précédente l’humidité était entre 42 % et 97 %, sachant que la donnée est en exprimer en pourcentage d’humidité relative, je prévois donc d’utiliser 7 bits ce qui correspond à un maximum possible de 123.

##### Température
Pour la température j’utilise 6 bits pour la valeur et un 1 bit consacré au signe, du fait que le capteur peut aller de -50 à +50 °C, les 7 bits me permettent donc une amplitude de -63 à +63.

##### Radiation
Pour la radiation, n'ayant aucune données sur internet et de l'année précédente, j'ai donc décidé de m'en occupé en dernier et de lui attribuer le nombre de bits restant, soit 15 bits

>#### Diagramme de classe

<img src="/img/diagramme_classe_sigfox.png" title="Diagramme de classe Sigfox">

La partie envoi c'est fait a l'aide de la bibliothèque créé par notre professeur, Cette bibliothèque s'appelle "Sigfox.h".

Celle-ci nous permet de réaliser toutes les procédures concernant le module SigFox, de l’initialisation de la liaison jusqu’à l’envoi, ainsi que l’obtention des données internes au composant, telles que son id. Nous avons utilisé la méthode «begin()» pour la liaison et la méthode «envoyer()» pour l’envoi de la trame en hexadécimal. J'ai créé la bibliothèque "sigfoxBallon.h" qui permet réalise l'encodage de la trame avec la méthode «coderTrame()», qui prend en paramètre la structures de données. 

<hr>

### Sauvegarder mesures et positions

La sauvegarde c'est fait via une carte micro SD, de plus le TTGO T1 possède un lecteur.
La sauvegarde c'est réalisé a l'aide fichier CSV, du fait que sont format est simple et qu'il permet de fusionnées une grandes quantités de données.

Rappelle du format CSV
````csv
datetime ; altitude ; longitude ; latitude ; radiation ; temperature ; pression ; humidite
2022-06-03 07:57:07 ; 28.70 ; 0.20392500 ; 47.99525833 ; 3.25 ; 25.58 ; 1007.77 ; 50.05
````

Pour l'enregistrement toutes les données de la structure de données on été converti sous forme de chaine de caractère, pour l'écriture dans le fichier.

#### Classe Utilisé

La tache « Sauvegarder mesure et position » a besoin de trois bibliothèques pour fonctionner « SPI », « SD », et « FS » . Les méthodes « begin » des classes « SPI » et « SD » permettent d’initialiser la liaison, la classe « FS » permet d’ouvrir le fichier avec la méthode « open », d’écrire avec « println » et de le fermer avec « close ».

<!--<img src="" title="Diagramme de classe">-->

<hr>

### Afficher page web admin

Bibliothèques utilisées :
 - WiFi
 - ESPAsyncWebServer
 - ArduinoJson
 - AsyncTCP *(nécessaire pour le bon fonctionnement de ESPAsyncWebServer avec l'ESP32)*

Lors de la réalisation de la page web, grâce à l'installation de PlatformIO, j’ai pu utiliser l’outil SPIFFS avec l’IDE Netbeans, ce qui ma permit de pouvoir créer des fichiers, HTML, JavaScript et CSS, dans le fichier « data » que j’ai ensuite pu compiler dans la mémoire interne de l'ESP32 avec la commande ``pio run --target uploadfs``.

Pour l’affichage de la page web, j'ai tout d’abord créer un point d’accès WiFi, pour la création de celui-ci, j'ai utilisé la bibliothèque ``WiFi``, la principale méthode à utiliser est ***«&nbsp;softAP&nbsp;»*** , elle permet de créer le point d’accès WiFi en prenant en paramètre le SSID et le mot de passe, de plus on peut ne pas mettre de mot de passe en mettant ***«&nbsp;NULL&nbsp;»*** en paramètre pour le mot de passe. 

Par ailleurs j'ai utilisé la méthode ***«&nbsp;softAPIP&nbsp;»*** qui m'a permis de récupérer l’adresse IP de l’ESP32 via une console en liaison série, cependant si l’adresse n'est pas changé celle-ci sera par défaut 192.168.4.1, et peut être modifiée via la méthode ***«&nbsp;softAPConfig&nbsp;»*** , cette méthode permet aussi de modifier aussi la passerelle par défaut et le masque de sous réseau.

Une fois le WiFi fonctionnel, j'ai crée un serveur web avec la bibliothèque ``ESPAsyncWebServer``, cette bibliothèque permet de gérer les requêtes asynchrones ce qui permet d’avoir un affichage dynamique. 
J'ai utilisé principalement les méthodes ***«&nbsp;begin&nbsp;»*** et ***«&nbsp;on&nbsp;»*** . La méthode ***«&nbsp;on&nbsp;»*** permet d'initialisé les routes vers les URL, elle prend en premier paramètre une URL, puis la méthode de requête GET ou POST, et suivi du code qui va être exécuté quand le client fera la requête à l'URL précédemment. De plus la bibliothèque ESPAsyncWebServer est compatible avec le système de fichiers SPIFFS ce qui permet de lier les fichiers de la page web à une URL.

> C++
```cpp
// initialisation du lien vers le fichier "index.html" présent dans le fichier data
server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
	request->send(SPIFFS, "/index.html", "text/html");
});
```

Pour l’actualisation des données, le client envoi des requêtes GET en boucle à l’aide d’un timer présent dans un JavaScript avec comme destination l'URL ***"/recupererDonnee"*** .

>Javascript
```javascript
setInterval(function recupererDonnee() {
	$.getJSON("/recupererDonnee")
		.done(function (resultat, status, xhr) {
			console.log(resultat);
			$("#longitude").text(resultat.longitude);
			$("#latitude").text(resultat.latitude);
			$("#altitude").text(resultat.altitude);
			$("#radiation").text(resultat.radiation);
			$("#temperature").text(resultat.temperature);
			$("#humidite").text(resultat.humidite);
			$("#pression").text(resultat.pression);
		})
		.fail(function (xhr, status, error) {
			console.log(xhr);
			console.log(status);
			console.log(error);
		});
}, 1000);
```
Par la suite le serveur envoi en réponse les données au format JSON avec la bibliothèque ``ArduinoJson``. 

Tout d'abord il y a l'initialisation de la réponse au format JSON, ensuite la suite la fonction ***"&nbsp;DynamicJsonDocument&nbsp;"*** alloue de l'espace a la variable nommé "json", qui récupère les différentes variables de la structures de données. Par la suite la fonction ***"&nbsp;serializeJson&nbsp;"*** sérialise la variable « json » vers un pointeur de « response » pour créer un document JSON minifié , c'est-à-dire un document sans espaces ni saut de ligne entre les valeurs pour éviter de surcharger le serveur lors de l’envoi, et pour finir on envoie le JSON minifié.

>C++
```cpp
// fonction recupererDonnee recupére les informations présentent dans la
// structure de données pour les envoyer au format JSON pour qu'il soit ensuite
// affiché sur la page Web
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
```
>### Interface

<img src="/img/interface%20web%20admin.png" title="interface Page Web Admin">

<hr>

## Amélioration a apporter

>### Sauvegarder mesures et position

Une amélioration serait a apporter avec la gestion de la date pour la création de fichier pour éviter tout problèmes de suppression de fichier sur la carte SD lors d'un redémarrage imprévue de l'ESP32.

>### Émettre trame SigFox

Pour cette tâche, il faudrait retirer un bit a une variable pour l'attribuer a l'altitude et quelle soit ne soit plus sur 15 bits mais sur 16 bits, car pendant son voyage le ballon a dépassé l'altitude 32767 qui était la valeurs maximal recevable.


## Informations Complémentaires

[Philippe SIMIER](https://github.com/PhilippeSimier) 
  -  [Installation de PlatformIO sur l'IDE Netbeans ](https://github.com/PhilippeSimier/Esp32/tree/master/00_install_EDI)
  -  [Ballon stratosphérique APRS](https://github.com/PhilippeSimier/Ballon_stratospherique_aprs_2022)

