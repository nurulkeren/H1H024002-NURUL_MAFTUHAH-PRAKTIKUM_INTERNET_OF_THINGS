#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


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


// Topic untuk menerima perintah
const char* topicPerintah = "unsoed/tk245004/kelompok1/perintah";


// =============================
// KONFIGURASI LED
// =============================

const int ledPin = 26;



// Membuat objek koneksi WiFi
WiFiClient espClient;


// Membuat objek MQTT Client
PubSubClient client(espClient);



// =================================================
// CALLBACK MQTT
// Menerima pesan dari topic subscribe
// =================================================

void callback(char* topic, byte* payload, unsigned int length) {


  String pesan = "";


  // Mengubah payload menjadi String
  for (unsigned int i = 0; i < length; i++) {

    pesan += (char)payload[i];

  }



  Serial.print("Pesan diterima: ");
  Serial.println(pesan);



  // =============================
  // DESERIALISASI JSON
  // =============================

  JsonDocument doc;


  DeserializationError error = deserializeJson(doc, pesan);



  // Mengecek JSON valid atau tidak
  if (error) {


    Serial.println("Parsing JSON gagal");


    return;


  }



  // Mengambil nilai perintah
  const char* perintah = doc["perintah"];



  // =============================
  // KENDALI LED
  // =============================


  if (String(perintah) == "ON") {


    digitalWrite(ledPin, HIGH);


    Serial.println("LED menyala");


  }


  else if (String(perintah) == "OFF") {


    digitalWrite(ledPin, LOW);


    Serial.println("LED mati");


  }


  else {


    Serial.println("Perintah tidak dikenali");


  }


}



// =================================================
// KONEKSI WIFI
// =================================================

void hubungkanWiFi() {


  WiFi.begin(ssid, password);


  Serial.print("Menghubungkan WiFi");


  while (WiFi.status() != WL_CONNECTED) {


    delay(500);

    Serial.print(".");


  }


  Serial.println();

  Serial.println("WiFi berhasil terhubung");


}



// =================================================
// KONEKSI MQTT
// =================================================

void hubungkanMQTT() {


  while (!client.connected()) {


    Serial.println("Menghubungkan MQTT...");


    String clientID = "ESP32Client-" + String(random(0xffff), HEX);



    if (client.connect(clientID.c_str())) {


      Serial.println("MQTT berhasil terhubung");


      client.subscribe(topicPerintah);


      Serial.print("Subscribe topic: ");

      Serial.println(topicPerintah);


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


}
