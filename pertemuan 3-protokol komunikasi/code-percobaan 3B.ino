#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Menentukan nama WiFi dan password
const char* ssid = "TECNOPOVA6";
const char* password = "hurufbesar";

// Menentukan alamat broker MQTT
const char* mqttServer = "broker.hivemq.com";

// Menentukan port MQTT
const int mqttPort = 1883;

// Menentukan topic MQTT yang digunakan
const char* mqttTopic = "unsoed/tk245004/kelompokAnda/sensor";

// Membuat objek WiFiClient
WiFiClient espClient;

// Membuat objek PubSubClient menggunakan koneksi WiFi
PubSubClient client(espClient);

void hubungkanWiFi() {

  // Memulai koneksi ESP32 ke jaringan WiFi
  WiFi.begin(ssid, password);

  // Menampilkan proses koneksi
  Serial.print("Menghubungkan ke WiFi");

  // Menunggu sampai ESP32 berhasil terhubung
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung!");
}

void hubungkanMQTT() {

  // Mengulang proses selama ESP32 belum terhubung ke broker
  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    // Membuat client ID secara acak
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    // Mencoba menghubungkan ESP32 ke broker MQTT
    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

    } else {

      // Menampilkan kode status jika koneksi gagal
      Serial.print("gagal, rc=");
      Serial.print(client.state());

      // Menunggu 2 detik sebelum mencoba kembali
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {

  // Memulai komunikasi Serial Monitor
  Serial.begin(115200);

  // Menghubungkan ESP32 ke WiFi
  hubungkanWiFi();

  // Menentukan server MQTT dan port yang digunakan
  client.setServer(mqttServer, mqttPort);
}

void loop() {

  // Mengecek koneksi MQTT
  if (!client.connected()) {
    hubungkanMQTT();
  }

  // Menjaga koneksi MQTT tetap aktif
  client.loop();

  // Membuat data sensor dalam format JSON
  JsonDocument doc;

  // Menambahkan data suhu
  doc["suhu"] = 28.5;

  // Menambahkan data kelembaban
  doc["kelembaban"] = 65.0;

  // Membuat buffer untuk menyimpan JSON
  char buffer[128];

  // Mengubah objek JSON menjadi teks
  serializeJson(doc, buffer);

  // Mempublikasikan data ke topic MQTT
  client.publish(mqttTopic, buffer);

  // Menampilkan informasi data yang dikirim
  Serial.print("Data terkirim ke topic ");
  Serial.print(mqttTopic);
  Serial.print(": ");
  Serial.println(buffer);

  // Publish data setiap 5 detik
  delay(5000);
}
