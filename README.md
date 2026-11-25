# Système intelligent de surveillance et de détection pour environnement industriel

## Présentation du projet

Ce projet consiste à réaliser un système de surveillance d'un
environnement industriel à l'aide d'une carte **ESP32** et de plusieurs
capteurs.

Le système mesure la température et l'humidité, détecte les mouvements
et déclenche une alarme sonore lorsque la température dépasse un seuil
défini. Les informations sont affichées en temps réel sur un écran OLED.

Ce prototype constitue une première étape vers un système de
surveillance industrielle connecté. La version actuelle utilise une
détection basée sur un seuil fixe ; elle ne comporte pas encore de
modèle de Machine Learning.

## Problématique

Dans un environnement industriel, des conditions environnementales
anormales ou une présence non détectée peuvent nécessiter une
intervention rapide. Il est donc utile de surveiller certains paramètres
et d'avertir l'utilisateur lorsqu'une situation dépasse les limites
définies.

**Question :** comment concevoir un système embarqué capable de
surveiller un environnement, de détecter des situations anormales et
d'avertir l'utilisateur en temps réel ?

## Objectifs

-   Mesurer la température et l'humidité ambiantes.
-   Détecter les mouvements à l'aide d'un capteur PIR.
-   Comparer la température à un seuil de **25 °C**.
-   Déclencher une alarme sonore en cas de dépassement du seuil.
-   Afficher les mesures et les états du système sur un écran OLED.
-   Centraliser l'acquisition et le traitement des données avec l'ESP32.

## Matériel utilisé

  Composant        Rôle
  ---------------- -------------------------------------------
  ESP32            Acquisition, traitement et commande
  DHT11            Mesure de la température et de l'humidité
  PIR HC-SR501     Détection de mouvement
  Buzzer actif     Avertissement sonore
  Écran OLED I2C   Affichage des mesures et états
  Potentiomètre    Lecture d'une valeur analogique

## Branchement des composants

  Composant       Broche ESP32
  --------------- --------------
  DHT11 DATA      GPIO 4
  PIR OUT         GPIO 23
  Buzzer actif    GPIO 19
  Potentiomètre   GPIO 33
  OLED SDA        GPIO 21
  OLED SCL        GPIO 22

Les composants partagent une masse commune (**GND**). Le DHT11 et
l'écran OLED sont alimentés en 3,3 V. Alimenter le module PIR
conformément aux caractéristiques de sa carte.

> Vérifier les tensions admissibles et le brochage exact des modules
> utilisés avant tout branchement réel.

## Fonctionnement

1.  L'ESP32 initialise les capteurs et l'écran OLED.
2.  Le DHT11 mesure la température et l'humidité.
3.  Le capteur PIR indique si un mouvement est détecté.
4.  L'ESP32 lit la valeur analogique du potentiomètre.
5.  La température mesurée est comparée au seuil de 25 °C.
6.  Si la température dépasse 25 °C, le buzzer est activé ; sinon, il
    reste désactivé.
7.  Les valeurs et les états sont affichés sur l'écran OLED.
8.  Le cycle est répété en continu.

## Algorithme de détection

``` text
Initialiser l'ESP32, les capteurs et l'écran OLED

Répéter continuellement :
    Lire la température et l'humidité
    Lire l'état du capteur PIR
    Lire la valeur du potentiomètre

    Si température > 25 °C :
        Activer le buzzer
        Indiquer une alerte de température
    Sinon :
        Désactiver le buzzer

    Afficher les mesures et les états sur l'écran OLED
```

## Technologies et bibliothèques

-   **Arduino IDE** --- programmation de l'ESP32 en C/C++.
-   **Bibliothèque DHT** --- lecture du capteur DHT11.
-   **Adafruit GFX** --- fonctions graphiques pour l'écran.
-   **Adafruit SSD1306** --- gestion de l'écran OLED SSD1306.
-   **I2C** --- communication entre l'ESP32 et l'écran OLED.

## Tests à effectuer

-   Vérifier l'affichage de la température et de l'humidité.
-   Vérifier la détection de mouvement par le PIR.
-   Vérifier que le buzzer s'active lorsque la température dépasse 25
    °C.
-   Vérifier que le buzzer s'arrête lorsque la température revient à 25
    °C ou moins.
-   Vérifier que les informations de l'écran OLED sont actualisées.

## Applications possibles

-   Surveillance de locaux techniques.
-   Surveillance de petits ateliers.
-   Surveillance de salles contenant des équipements électroniques.
-   Surveillance de certains espaces de stockage.
-   Détection de mouvement dans une zone surveillée.

## Améliorations futures

-   Ajouter une connexion Wi-Fi pour transmettre les mesures.
-   Créer un tableau de bord avec Streamlit ou Node-RED.
-   Enregistrer les mesures dans une base de données.
-   Envoyer des notifications en cas d'alerte.
-   Commander automatiquement un ventilateur.
-   Ajouter un historique et des statistiques.
-   Collecter des données et étudier l'ajout d'un modèle de Machine
    Learning pour détecter des anomalies.

## Limites actuelles

-   Le seuil d'alerte est fixe et défini à 25 °C.
-   La détection repose sur des règles simples, pas sur un modèle
    entraîné.
-   Le système surveille des paramètres environnementaux ; il ne mesure
    pas directement l'état mécanique d'un moteur.
-   Le potentiomètre est lu, mais sa valeur n'est pas utilisée pour
    déclencher l'alarme dans la version actuelle.

## Conclusion

Ce prototype met en pratique les principes des systèmes embarqués et de
la surveillance par capteurs. Il permet de mesurer des paramètres
environnementaux, de détecter un mouvement et de signaler un dépassement
de température. Il peut ensuite évoluer vers une solution connectée avec
tableau de bord, stockage des données et détection intelligente des
anomalies.

------------------------------------------------------------------------

**Projet réalisé avec ESP32 --- IoT et systèmes embarqués**
