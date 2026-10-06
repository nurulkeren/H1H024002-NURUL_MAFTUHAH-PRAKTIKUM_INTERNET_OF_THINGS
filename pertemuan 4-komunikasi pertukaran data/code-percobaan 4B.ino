#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>


// =============================
// KONFIGURASI WIFI
// =============================

const char* ssid = "TECNO POVA 6";
const char* password = "hurufbesar";


// =============================
// KONFIGURASI MQTT
// =============================

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;


// Topic publish data suhu
const char* topicData = "unsoed/tk245004/kelompok1/data";


// Topic subscribe perintah
const char* topicPerintah = "unsoed/tk245004/kelompok1/perintah";


// =============================
// SENSOR DAN LED
// =============================

#define DHTPIN 4
#define DHTTYPE DHT11


const int ledPin = 26;



DHT dht(DHTPIN, DHTTYPE);



WiFiClient espClient;


PubSubClient client(espClient);



// Interval publish data
unsigned long waktuPublish = 0;

const long interval = 5000;



// =================================================
// CALLBACK MQTT
// =================================================

void callback(char* topic, byte* payload, unsigned int length) {


  String pesan = "";


  for (unsigned int i = 0; i < length; i++) {


    pesan += (char)payload[i];


  }



  Serial.print("Pesan diterima: ");

  Serial.println(pesan);



  JsonDocument doc;



  DeserializationError error = deserializeJson(doc, pesan);



  if (error) {


    Serial.println("JSON error");


    return;


  }



  const char* perintah = doc["perintah"];



  if (String(perintah) == "ON") {


    digitalWrite(ledPin, HIGH);


    Serial.println("LED menyala");


  }


  else if (String(perintah) == "OFF") {


    digitalWrite(ledPin, LOW);


    Serial.println("LED mati");


  }


}



// =================================================
// WIFI
// =================================================

void hubungkanWiFi() {


  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {


    delay(500);

    Serial.print(".");


  }


  Serial.println();

  Serial.println("WiFi terhubung");


}



// =================================================
// MQTT
// =================================================

void hubungkanMQTT() {


  while (!client.connected()) {


    String clientID = "ESP32Client-" + String(random(0xffff), HEX);



    if (client.connect(clientID.c_str())) {


      Serial.println("MQTT terhubung");


      client.subscribe(topicPerintah);


    }


    else {


      delay(2000);


    }


  }


}



// =================================================
// SETUP
// =================================================

void setup() {


  Serial.begin(115200);



  dht.begin();



  pinMode(ledPin, OUTPUT);


  digitalWrite(ledPin, LOW);



  hubungkanWiFi();



  client.setServer(mqttServer, mqttPort);



  client.setCallback(callback);


}



// =================================================
// LOOP
// =================================================

void loop() {


  if (!client.connected()) {


    hubungkanMQTT();


  }



  client.loop();




  // =============================
  // PUBLISH DATA SUHU
  // =============================


  unsigned long sekarang = millis();



  if (sekarang - waktuPublish >= interval) {


    waktuPublish = sekarang;



    float suhu = dht.readTemperature();



    if (isnan(suhu)) {


      Serial.println("Sensor gagal dibaca");


      return;


    }



    JsonDocument doc;



    doc["suhu"] = suhu;



    String dataJSON;



    serializeJson(doc, dataJSON);



    client.publish(topicData, dataJSON.c_str());



    Serial.print("Data terkirim: ");

    Serial.println(dataJSON);


  }


}
