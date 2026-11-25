#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= WIFI =================

const char* ssid = "DESKTOP-SOR3HH2 0033";
const char* password = "1234567899";

// ================= FIREBASE =================

const char* firebaseURL =
"https://industrialmonitoringesp32-default-rtdb.europe-west1.firebasedatabase.app/sensors.json";

// ================= CAPTEURS =================

#define DHTPIN 4
#define DHTTYPE DHT11

#define PIR_PIN 23
#define BUZZER_PIN 19
#define POT_PIN 33

DHT dht(DHTPIN, DHTTYPE);

// ================= OLED =================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// ================= VARIABLES =================

unsigned long previousMillis = 0;
const unsigned long interval = 3000;

int mode = 1;

// ================= SETUP =================

void setup() {

  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  analogReadResolution(12);

  dht.begin();

  Wire.begin(21, 22);

  // Initialisation OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur OLED !");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("INDUSTRIAL MONITOR");
  display.println("Initialisation...");
  display.display();

  // Connexion WiFi

  WiFi.begin(ssid, password);

  Serial.print("Connexion WiFi");

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed!");
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  if (WiFi.status() == WL_CONNECTED) {
    display.println("WiFi CONNECTE");
  } else {
    display.println("WiFi DECONNECTE");
  }

  display.display();

  delay(2000);
}

// ================= LOOP =================

void loop() {

  // 1. Lecture des capteurs

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int mouvement = digitalRead(PIR_PIN);
  int potValue = analogRead(POT_PIN);

  // 2. Selection du mode

  if (potValue < 1365) {
    mode = 1;
  }
  else if (potValue < 2730) {
    mode = 2;
  }
  else {
    mode = 3;
  }

  // 3. Detection de surchauffe

  bool surchauffe = !isnan(temperature) &&
                    temperature > 25;

  // 4. Gestion des 3 modes

  bool alarme = false;

  if (mode == 1) {
    alarme = (mouvement == HIGH);
  }
  else if (mode == 2) {
    alarme = surchauffe;
  }
  else if (mode == 3) {
    alarme = (mouvement == HIGH) || surchauffe;
  }

  // 5. Commande du buzzer

  digitalWrite(BUZZER_PIN, alarme ? HIGH : LOW);

  // 6. Affichage Serial Monitor

  Serial.println("\n----------------------");
  Serial.println("INDUSTRIAL MONITOR");
  Serial.println("----------------------");

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  } else {
    Serial.println("Erreur DHT11 Temperature");
  }

  if (!isnan(humidity)) {
    Serial.print("Humidite: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Erreur DHT11 Humidite");
  }

  Serial.print("Mouvement: ");
  Serial.println(mouvement ? "DETECTE" : "ABSENT");

  Serial.print("Potentiometre: ");
  Serial.println(potValue);

  Serial.print("Mode: ");
  Serial.println(mode);

  Serial.print("Alarme: ");
  Serial.println(alarme ? "ACTIVE" : "DESACTIVEE");

  // 7. Affichage OLED

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("INDUSTRIAL MONITOR");
  display.println("------------------");

  if (!isnan(temperature)) {
    display.print("Temp: ");
    display.print(temperature);
    display.println(" C");
  } else {
    display.println("Temp: Erreur");
  }

  if (!isnan(humidity)) {
    display.print("Hum: ");
    display.print(humidity);
    display.println(" %");
  } else {
    display.println("Hum: Erreur");
  }

  display.print("PIR: ");
  display.println(mouvement ? "DETECTE" : "ABSENT");

  display.print("Mode: ");
  display.println(mode);

  display.print("Etat: ");
  display.println(alarme ? "ALERTE !" : "NORMAL");

  display.display();

  // 8. Envoi des donnees vers Firebase
  // Toutes les 3 secondes

  if (millis() - previousMillis >= interval) {

    previousMillis = millis();

    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      http.begin(firebaseURL);
      http.addHeader("Content-Type", "application/json");

      String tempValue = isnan(temperature)
                         ? "null" : String(temperature, 1);

      String humValue = isnan(humidity)
                        ? "null" : String(humidity, 1);

      String json = "{";

      json += "\"temperature\":" + tempValue + ",";
      json += "\"humidite\":" + humValue + ",";
      json += "\"mouvement\":";
      json += (mouvement == HIGH ? "true" : "false");
      json += ",";
      json += "\"potentiometre\":" + String(potValue) + ",";
      json += "\"mode\":" + String(mode) + ",";
      json += "\"alarme\":";
      json += (alarme ? "true" : "false");

      json += "}";

      int httpCode = http.PUT(json);

      Serial.println("\n--- FIREBASE ---");
      Serial.println(json);

      Serial.print("HTTP Code: ");
      Serial.println(httpCode);

      if (httpCode > 0) {
        Serial.println(http.getString());
      } else {
        Serial.println("Erreur de connexion Firebase");
      }

      http.end();

    } else {
      Serial.println("WiFi non connecte");
    }
  }

  delay(100);
}