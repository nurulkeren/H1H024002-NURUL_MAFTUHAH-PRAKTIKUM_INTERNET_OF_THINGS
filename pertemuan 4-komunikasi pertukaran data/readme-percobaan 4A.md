# Modul 4 - Percobaan 4A
# Subscribe dan Deserialisasi Data JSON untuk Kendali Aktuator


## 1. Detail Percobaan

Percobaan ini membahas komunikasi data menggunakan protokol MQTT dengan metode
subscribe pada ESP32. Pada percobaan ini ESP32 digunakan untuk menerima pesan
berupa data JSON dari broker MQTT yang kemudian diproses menggunakan metode
deserialisasi.

Data JSON yang diterima berisi perintah untuk mengendalikan aktuator berupa LED.
ESP32 terlebih dahulu melakukan koneksi ke jaringan WiFi TECNO POVA 6 kemudian
terhubung dengan broker MQTT HiveMQ.

Setelah berhasil terhubung, ESP32 melakukan subscribe pada topic tertentu untuk
menunggu pesan masuk. Ketika pesan JSON diterima, data tersebut akan dilakukan
proses parsing sehingga nilai perintah dapat digunakan untuk mengatur kondisi
LED.


## Spesifikasi yang Diharapkan

1. ESP32 berhasil terhubung dengan jaringan WiFi.
2. ESP32 berhasil terhubung dengan broker MQTT.
3. ESP32 berhasil melakukan subscribe pada topic perintah.
4. ESP32 berhasil menerima data JSON dari MQTT Explorer.
5. Proses deserialisasi JSON berhasil dilakukan.
6. LED dapat menyala dan mati sesuai perintah yang diterima.
7. Serial Monitor menampilkan pesan yang diterima dan hasil parsing data.


---

# 2. Library / Dependencies


Library yang diperlukan:


```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
```
Fungsi Library
| Library | Fungsi |
|---|---|
| `WiFi.h` | Digunakan untuk menghubungkan ESP32 dengan jaringan WiFi |
| `PubSubClient.h` | Digunakan untuk komunikasi MQTT publish dan subscribe |
| `ArduinoJson.h` | Digunakan untuk membuat dan melakukan parsing data JSON |

# 3. penjelasan program
```cpp
// Memanggil library WiFi untuk koneksi ESP32 dengan jaringan internet
#include <WiFi.h>

// Memanggil library PubSubClient untuk komunikasi MQTT
#include <PubSubClient.h>

// Memanggil library ArduinoJson untuk proses parsing data JSON
#include <ArduinoJson.h>


// =============================
// KONFIGURASI WIFI
// =============================

// Nama jaringan WiFi yang digunakan
const char* ssid = "NAMA_WIFI_ANDA";

// Password WiFi
const char* password = "PASSWORD_WIFI_ANDA";


// =============================
// KONFIGURASI MQTT
// =============================

// Alamat broker MQTT HiveMQ
const char* mqttServer = "broker.hivemq.com";

// Port komunikasi MQTT
const int mqttPort = 1883;


// Topic MQTT yang digunakan untuk menerima perintah
// Topic harus sama dengan topic yang digunakan pada MQTT Explorer
const char* topicPerintah = "iot/esp32/perintah";


// =============================
// KONFIGURASI LED
// =============================

// Pin LED yang digunakan sebagai aktuator
const int ledPin = 2;


// Membuat objek koneksi WiFi
WiFiClient espClient;


// Membuat objek MQTT Client
PubSubClient client(espClient);



// =================================================
// FUNGSI CALLBACK MQTT
// Fungsi ini otomatis dijalankan ketika ada pesan baru
// masuk dari topic yang telah di-subscribe
// =================================================

void callback(char* topic, byte* payload, unsigned int length) {


  // Variabel untuk menyimpan pesan JSON yang diterima
  String pesan = "";


  // Mengubah data byte payload menjadi String
  for (unsigned int i = 0; i < length; i++) {

    pesan += (char)payload[i];

  }


  // Menampilkan topic dan pesan yang diterima
  Serial.print("Pesan diterima dari topic: ");
  Serial.println(topic);

  Serial.print("Data JSON: ");
  Serial.println(pesan);



  // =====================================
  // PROSES DESERIALISASI JSON
  // Mengubah teks JSON menjadi object
  // yang dapat dibaca oleh ESP32
  // =====================================

  JsonDocument doc;


  // Melakukan proses parsing JSON
  DeserializationError error = deserializeJson(doc, pesan);



  // Mengecek apakah proses parsing berhasil
  if (error) {

    Serial.print("Parsing JSON gagal: ");
    Serial.println(error.c_str());

    return;

  }



  // Mengambil nilai dari key "perintah"
  // Contoh data:
  // {"perintah":"ON"}

  const char* perintah = doc["perintah"];



  // =====================================
  // KENDALI AKTUATOR LED
  // =====================================


  // Jika perintah yang diterima ON
  maka LED menyala
  if (String(perintah) == "ON") {


    digitalWrite(ledPin, HIGH);


    Serial.println("LED menyala");


  }


  // Jika perintah yang diterima OFF
  maka LED mati
  else if (String(perintah) == "OFF") {


    digitalWrite(ledPin, LOW);


    Serial.println("LED mati");


  }


  // Jika perintah tidak sesuai
  else {


    Serial.println("Perintah tidak dikenali");


  }

}



// =================================================
// FUNGSI KONEKSI WIFI
// Digunakan untuk menghubungkan ESP32
// dengan jaringan WiFi
// =================================================

void hubungkanWiFi() {


  // Memulai koneksi WiFi
  WiFi.begin(ssid, password);


  Serial.print("Menghubungkan WiFi");


  // Menunggu sampai WiFi berhasil tersambung
  while (WiFi.status() != WL_CONNECTED) {


    delay(500);

    Serial.print(".");

  }


  Serial.println();

  Serial.println("WiFi berhasil terhubung");


  // Menampilkan alamat IP ESP32
  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());


}



// =================================================
// FUNGSI KONEKSI MQTT
// Digunakan untuk menghubungkan ESP32
// dengan broker MQTT
// =================================================

void hubungkanMQTT() {


  // Melakukan perulangan sampai MQTT berhasil terhubung
  while (!client.connected()) {


    Serial.println("Menghubungkan MQTT...");


    // Membuat ID client MQTT secara random
    String clientID = "ESP32Client-" + String(random(0xffff), HEX);



    // Melakukan koneksi ke broker MQTT
    if (client.connect(clientID.c_str())) {


      Serial.println("MQTT berhasil terhubung");



      // Subscribe topic setelah koneksi berhasil
      client.subscribe(topicPerintah);



      Serial.print("Subscribe topic: ");

      Serial.println(topicPerintah);


    }


    else {


      Serial.print("MQTT gagal, kode: ");

      Serial.println(client.state());



      // Menunggu sebelum mencoba koneksi ulang
      delay(2000);


    }


  }


}



// =================================================
// SETUP
// Dijalankan sekali ketika ESP32 pertama kali aktif
// =================================================

void setup() {


  // Membuka komunikasi serial monitor
  Serial.begin(115200);



  // Mengatur pin LED sebagai output
  pinMode(ledPin, OUTPUT);



  // Kondisi awal LED mati
  digitalWrite(ledPin, LOW);



  // Menghubungkan ESP32 ke WiFi
  hubungkanWiFi();



  // Mengatur alamat broker MQTT
  client.setServer(mqttServer, mqttPort);



  // Menentukan fungsi callback untuk menerima pesan
  client.setCallback(callback);


}



// =================================================
// LOOP
// Program yang dijalankan terus menerus
// =================================================

void loop() {


  // Jika koneksi MQTT terputus,
  // lakukan koneksi ulang

  if (!client.connected()) {


    hubungkanMQTT();


  }



  // Menjaga koneksi MQTT dan memeriksa
  // apakah ada pesan baru yang masuk

  client.loop();


}
```
###a. Inisialisasi Library
Pada bagian awal program terdapat tiga library utama yaitu WiFi.h, PubSubClient.h, dan ArduinoJson.h. Library WiFi.h digunakan agar ESP32 dapat terhubung dengan jaringan internet. Library PubSubClient.h digunakan untuk menjalankan komunikasi MQTT seperti melakukan koneksi, subscribe topic, dan menerima pesan dari broker. Sedangkan library ArduinoJson.h digunakan untuk melakukan proses deserialisasi data JSON yang diterima dari MQTT menjadi data yang dapat dibaca oleh program.
###b. Konfigurasi WiFi dan MQTT
Program menentukan nama WiFi, password, alamat broker MQTT, port komunikasi, serta topic yang digunakan untuk menerima perintah. Topic harus dibuat sama antara ESP32 dan aplikasi MQTT Explorer agar pesan dapat diterima oleh perangkat.
###c. Fungsi Callback
Fungsi callback() merupakan fungsi utama pada mekanisme subscribe MQTT. Fungsi ini akan berjalan secara otomatis ketika broker menerima pesan baru pada topic yang telah didaftarkan. Data yang diterima dalam bentuk byte kemudian diubah menjadi String agar dapat diproses sebagai format JSON.
Mekanisme callback dan penggunaan client.subscribe(topic) merupakan bagian utama dari proses subscribe MQTT karena perangkat hanya menunggu pesan masuk dari topic tertentu.     Modul Praktikum IoT 4 - Komunik…
###d. Proses Deserialisasi JSON
Data JSON yang diterima kemudian diproses menggunakan fungsi deserializeJson(). Proses ini mengubah data teks JSON menjadi object yang dapat diakses berdasarkan key tertentu.
Contoh data yang dikirim melalui MQTT Explorer:
```cpp
{
  "perintah":"ON"
}
```
Setelah proses deserialisasi berhasil, nilai "ON" dapat diambil menggunakan:
```cpp
doc["perintah"]
```
Kemudian nilai tersebut digunakan untuk menentukan kondisi LED.
###e. Kendali LED
Jika nilai perintah adalah "ON", maka ESP32 memberikan logika HIGH pada pin LED sehingga LED menyala. Sebaliknya jika nilai perintah adalah "OFF", maka ESP32 memberikan logika LOW sehingga LED mati.
###f. Fungsi client.loop()
Pada bagian loop(), fungsi client.loop() harus dijalankan secara terus menerus agar ESP32 tetap dapat memeriksa pesan MQTT yang masuk dan menjaga koneksi dengan broker. Modul juga menjelaskan bahwa fungsi loop MQTT harus dipanggil secara berkala agar perangkat tetap responsif terhadap pesan baru.
