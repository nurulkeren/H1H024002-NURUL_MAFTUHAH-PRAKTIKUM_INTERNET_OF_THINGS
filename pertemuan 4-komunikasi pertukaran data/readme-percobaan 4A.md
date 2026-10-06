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
### a. Inisialisasi Library
Pada bagian awal program terdapat tiga library utama yaitu WiFi.h, PubSubClient.h, dan ArduinoJson.h. Library WiFi.h digunakan agar ESP32 dapat terhubung dengan jaringan internet. Library PubSubClient.h digunakan untuk menjalankan komunikasi MQTT seperti melakukan koneksi, subscribe topic, dan menerima pesan dari broker. Sedangkan library ArduinoJson.h digunakan untuk melakukan proses deserialisasi data JSON yang diterima dari MQTT menjadi data yang dapat dibaca oleh program.
### b. Konfigurasi WiFi dan MQTT
Program menentukan nama WiFi, password, alamat broker MQTT, port komunikasi, serta topic yang digunakan untuk menerima perintah. Topic harus dibuat sama antara ESP32 dan aplikasi MQTT Explorer agar pesan dapat diterima oleh perangkat.
### c. Fungsi Callback
Fungsi callback() merupakan fungsi utama pada mekanisme subscribe MQTT. Fungsi ini akan berjalan secara otomatis ketika broker menerima pesan baru pada topic yang telah didaftarkan. Data yang diterima dalam bentuk byte kemudian diubah menjadi String agar dapat diproses sebagai format JSON.
Mekanisme callback dan penggunaan client.subscribe(topic) merupakan bagian utama dari proses subscribe MQTT karena perangkat hanya menunggu pesan masuk dari topic tertentu.     Modul Praktikum IoT 4 - Komunik…
### d. Proses Deserialisasi JSON
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
### e. Kendali LED
Jika nilai perintah adalah "ON", maka ESP32 memberikan logika HIGH pada pin LED sehingga LED menyala. Sebaliknya jika nilai perintah adalah "OFF", maka ESP32 memberikan logika LOW sehingga LED mati.
### f. Fungsi client.loop()
Pada bagian loop(), fungsi client.loop() harus dijalankan secara terus menerus agar ESP32 tetap dapat memeriksa pesan MQTT yang masuk dan menjaga koneksi dengan broker. Modul juga menjelaskan bahwa fungsi loop MQTT harus dipanggil secara berkala agar perangkat tetap responsif terhadap pesan baru.
# 4. penjelasan tiap fungsi

| No | Fungsi / Bagian Kode | Penjelasan |
|---|---|---|
| 1 | `#include <WiFi.h>` | Library yang digunakan untuk menghubungkan ESP32 dengan jaringan WiFi agar dapat melakukan komunikasi dengan broker MQTT melalui internet. |
| 2 | `#include <PubSubClient.h>` | Library yang digunakan untuk menjalankan komunikasi MQTT pada ESP32 seperti koneksi broker, publish, dan subscribe pesan. |
| 3 | `#include <ArduinoJson.h>` | Library yang digunakan untuk melakukan pengolahan data JSON, terutama proses deserialisasi data yang diterima dari MQTT. |
| 4 | `WiFiClient espClient` | Membuat objek koneksi WiFi yang digunakan sebagai jalur komunikasi ESP32 dengan broker MQTT. |
| 5 | `PubSubClient client(espClient)` | Membuat objek MQTT client yang digunakan untuk menjalankan fungsi komunikasi MQTT. |
| 6 | `setup()` | Fungsi yang dijalankan satu kali saat ESP32 pertama kali aktif untuk melakukan inisialisasi awal program. |
| 7 | `Serial.begin(115200)` | Mengaktifkan komunikasi serial dengan baud rate 115200 agar informasi proses program dapat ditampilkan pada Serial Monitor. |
| 8 | `pinMode(ledPin, OUTPUT)` | Mengatur pin LED sebagai output sehingga dapat dikendalikan oleh ESP32. |
| 9 | `digitalWrite(ledPin, LOW)` | Memberikan kondisi awal LED dalam keadaan mati. |
| 10 | `hubungkanWiFi()` | Fungsi yang digunakan untuk menghubungkan ESP32 dengan jaringan WiFi berdasarkan SSID dan password yang telah ditentukan. |
| 11 | `WiFi.begin(ssid, password)` | Memulai proses koneksi ESP32 dengan jaringan WiFi. |
| 12 | `hubungkanMQTT()` | Fungsi yang digunakan untuk menghubungkan ESP32 dengan broker MQTT HiveMQ. |
| 13 | `client.connect()` | Digunakan untuk melakukan koneksi antara ESP32 dengan broker MQTT. |
| 14 | `client.subscribe(topicPerintah)` | Digunakan untuk mendaftarkan ESP32 pada topic tertentu agar dapat menerima pesan atau perintah dari MQTT. |
| 15 | `client.setCallback(callback)` | Digunakan untuk menentukan fungsi callback yang akan dijalankan ketika pesan baru diterima. |
| 16 | `callback()` | Fungsi yang otomatis berjalan ketika ESP32 menerima pesan baru dari topic yang telah di-subscribe. |
| 17 | `payload` | Data mentah yang diterima dari broker MQTT dalam bentuk byte sebelum diubah menjadi String. |
| 18 | `String pesan` | Variabel yang digunakan untuk menyimpan data JSON yang diterima dari MQTT. |
| 19 | `deserializeJson(doc, pesan)` | Fungsi yang digunakan untuk melakukan proses deserialisasi, yaitu mengubah teks JSON menjadi object JSON agar nilai di dalamnya dapat digunakan oleh program. |
| 20 | `doc["perintah"]` | Digunakan untuk mengambil nilai dari key perintah pada data JSON yang diterima. |
| 21 | `digitalWrite(ledPin, HIGH)` | Memberikan logika HIGH pada pin LED sehingga LED menyala ketika menerima perintah ON. |
| 22 | `digitalWrite(ledPin, LOW)` | Memberikan logika LOW pada pin LED sehingga LED mati ketika menerima perintah OFF. |
| 23 | `loop()` | Fungsi yang berjalan secara berulang selama ESP32 aktif untuk menjalankan proses utama program. |
| 24 | `client.connected()` | Mengecek status koneksi ESP32 dengan broker MQTT. |
| 25 | `client.loop()` | Menjaga koneksi MQTT tetap aktif serta memeriksa pesan baru yang masuk dari topic subscribe. |
| 26 | `delay()` | Memberikan jeda waktu pada program, digunakan ketika proses percobaan koneksi ulang ke broker MQTT. |
# 5. penjelasan percabangan dan conditional

Pada program ini digunakan struktur percabangan `if`, `else if`, dan `else` untuk menentukan aksi yang dilakukan oleh ESP32 berdasarkan nilai perintah yang diterima melalui data JSON dari MQTT.

| No | Percabangan / Conditional | Penjelasan |
|---|---|---|
| 1 | `if (error)` | Digunakan untuk melakukan pengecekan apakah proses deserialisasi data JSON berhasil atau mengalami kesalahan. Jika terdapat error saat membaca JSON, program akan menampilkan pesan kesalahan pada Serial Monitor dan menghentikan proses callback menggunakan `return`. |
| 2 | `if (String(perintah) == "ON")` | Kondisi pertama digunakan untuk mengecek apakah nilai pada key `"perintah"` dalam data JSON bernilai `"ON"`. Jika kondisi terpenuhi, ESP32 akan memberikan logika HIGH pada pin LED sehingga LED menyala. |
| 3 | `else if (String(perintah) == "OFF")` | Kondisi kedua digunakan apabila kondisi sebelumnya tidak terpenuhi. Program akan mengecek apakah nilai perintah yang diterima adalah `"OFF"`. Jika benar, ESP32 akan memberikan logika LOW pada pin LED sehingga LED mati. |
| 4 | `else` | Digunakan sebagai kondisi terakhir apabila nilai perintah yang diterima bukan `"ON"` maupun `"OFF"`. Program akan menganggap perintah tidak dikenali dan menampilkan informasi pada Serial Monitor. |
| 5 | `if (!client.connected())` | Digunakan untuk mengecek kondisi koneksi MQTT. Jika ESP32 tidak lagi terhubung dengan broker, maka program akan menjalankan fungsi `hubungkanMQTT()` untuk melakukan koneksi ulang. |
| 6 | `while (WiFi.status() != WL_CONNECTED)` | Digunakan sebagai perulangan kondisi untuk memastikan ESP32 tetap mencoba menghubungkan diri ke jaringan WiFi hingga koneksi berhasil dilakukan. |
| 7 | `while (!client.connected())` | Digunakan untuk melakukan percobaan koneksi ulang ke broker MQTT selama ESP32 belum berhasil terhubung. |
# 6. jawaban pertanyaan praktikum
## 1. Diagram alur / Flowchart
```text
              ┌───────────────┐
              │     MULAI     │
              └───────┬───────┘
                      │
                      v
        ┌─────────────────────────┐
        │ Pesan baru diterima     │
        │ dari broker MQTT        │
        └───────────┬─────────────┘
                    │
                    v
        ┌─────────────────────────┐
        │ Fungsi callback()       │
        │ dipanggil               │
        └───────────┬─────────────┘
                    │
                    v
        ┌─────────────────────────┐
        │ Membaca data payload    │
        │ dari MQTT               │
        └───────────┬─────────────┘
                    │
                    v
        ┌─────────────────────────┐
        │ Mengubah payload byte   │
        │ menjadi String          │
        └───────────┬─────────────┘
                    │
                    v
        ┌─────────────────────────┐
        │ Deserialisasi JSON      │
        │ deserializeJson()       │
        └───────────┬─────────────┘
                    │
                    v
              ◇─────────────◇
             ╱ JSON valid?   ╲
            ◇───────────────◇
              │             │
            Tidak           Ya
              │             │
              v             v
 ┌──────────────────┐  ┌────────────────────┐
 │ Tampilkan pesan  │  │ Ambil nilai        │
 │ error parsing    │  │ "perintah" JSON    │
 │ Serial Monitor   │  └─────────┬──────────┘
 └────────┬─────────┘            │
          │                      v
          │              ◇────────────────◇
          │             ╱ Perintah = ON?  ╲
          │            ◇──────────────────◇
          │                │          │
          │              Ya          Tidak
          │                │          │
          │                v          v
          │       ┌────────────┐  ◇───────────────◇
          │       │ LED ON     │ ╱ Perintah = OFF? ╲
          │       │ HIGH       │◇─────────────────◇
          │       └─────┬──────┘      │        │
          │             │           Ya        Tidak
          │             │            │          │
          │             │            v          v
          │             │     ┌──────────┐ ┌───────────────┐
          │             │     │ LED OFF  │ │ Perintah      │
          │             │     │ LOW      │ │ tidak dikenali│
          │             │     └────┬─────┘ └───────┬───────┘
          │             │          │               │
          └─────────────┴──────────┴───────────────┘
                              │
                              v
                     ┌────────────────┐
                     │    SELESAI     │
                     └────────────────┘
```
---
## 2. Apa yang akan terjadi apabila pesan yang dipublikasikan bukan merupakan format JSON yang valid?
Jawaban:
Apabila pesan yang diterima bukan merupakan format JSON yang valid, maka proses deserialisasi menggunakan fungsi deserializeJson() akan mengalami kegagalan. Program akan mendeteksi adanya kesalahan melalui variabel error, kemudian menampilkan pesan kesalahan pada Serial Monitor.
Pada kondisi tersebut, program menjalankan perintah return sehingga proses berikutnya tidak dilanjutkan. Artinya, nilai perintah tidak akan dibaca dan LED tidak akan mengalami perubahan kondisi karena data yang diterima tidak dapat diproses.
## 3. Jelaskan mengapa fungsi client.subscribe() dipanggil di dalam fungsi hubungkanMQTT(), bukan di dalam setup()!
jawaban: 
Fungsi client.subscribe() diletakkan di dalam fungsi hubungkanMQTT() karena proses subscribe hanya dapat dilakukan apabila ESP32 sudah berhasil terhubung dengan broker MQTT.
Pada saat program pertama kali berjalan, fungsi setup() hanya melakukan proses inisialisasi seperti pengaturan pin, koneksi WiFi, konfigurasi broker, dan pendaftaran callback. Pada kondisi tersebut koneksi MQTT belum tentu sudah terbentuk sehingga proses subscribe belum dapat dilakukan.
Dengan menempatkan client.subscribe() setelah proses client.connect() berhasil, ESP32 hanya akan melakukan subscribe ketika koneksi MQTT sudah aktif. Selain itu, apabila koneksi MQTT terputus dan ESP32 melakukan koneksi ulang, proses subscribe akan dijalankan kembali secara otomatis.
## 4. Modifikasi program agar data JSON juga memuat nilai intensitas untuk mengatur kecerahan LED menggunakan PWM.
Jawaban:
```cpp
// Memanggil library WiFi untuk koneksi ESP32 dengan jaringan internet
#include <WiFi.h>

// Memanggil library MQTT untuk komunikasi publish dan subscribe
#include <PubSubClient.h>

// Memanggil library ArduinoJson untuk membaca data JSON
#include <ArduinoJson.h>


// ================================
// KONFIGURASI WIFI
// ================================

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";


// ================================
// KONFIGURASI MQTT
// ================================

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;


// Topic yang digunakan untuk menerima perintah
const char* topicPerintah = "iot/esp32/perintah";


// ================================
// KONFIGURASI LED PWM
// ================================

// Pin LED ESP32
const int ledPin = 2;


// Channel PWM yang digunakan ESP32
const int pwmChannel = 0;


// Frekuensi PWM
const int pwmFrequency = 5000;


// Resolusi PWM 8 bit
// Nilai PWM berada pada rentang 0 - 255
const int pwmResolution = 8;



// Membuat objek koneksi WiFi
WiFiClient espClient;


// Membuat objek MQTT Client
PubSubClient client(espClient);



// =================================================
// CALLBACK MQTT
// Fungsi ini berjalan otomatis ketika pesan baru
// diterima dari topic yang telah di-subscribe
// =================================================

void callback(char* topic, byte* payload, unsigned int length) {


  // Variabel untuk menyimpan pesan JSON
  String pesan = "";


  // Mengubah data byte menjadi String
  for (unsigned int i = 0; i < length; i++) {

    pesan += (char)payload[i];

  }



  // Menampilkan pesan yang diterima
  Serial.print("Pesan diterima: ");
  Serial.println(pesan);



  // ================================
  // DESERIALISASI JSON
  // ================================

  JsonDocument doc;


  DeserializationError error = deserializeJson(doc, pesan);



  // Mengecek apakah JSON valid
  if (error) {

    Serial.print("JSON Error: ");
    Serial.println(error.c_str());

    return;

  }



  // Mengambil nilai perintah dari JSON
  // Contoh:
  // {"perintah":"ON"}

  const char* perintah = doc["perintah"];



  // Mengambil nilai intensitas dari JSON
  // Contoh:
  // {"intensitas":200}

  int intensitas = doc["intensitas"];



  // ================================
  // PENGENDALIAN LED PWM
  // ================================


  // Jika perintah ON
  if (String(perintah) == "ON") {


    // Mengatur kecerahan LED sesuai nilai intensitas
    ledcWrite(pwmChannel, intensitas);


    Serial.print("LED ON - Intensitas: ");
    Serial.println(intensitas);


  }



  // Jika perintah OFF
  else if (String(perintah) == "OFF") {


    // Mengatur PWM menjadi 0 sehingga LED mati
    ledcWrite(pwmChannel, 0);


    Serial.println("LED OFF");


  }



  // Jika perintah tidak sesuai
  else {


    Serial.println("Perintah tidak dikenali");


  }


}



// =================================================
// FUNGSI KONEKSI WIFI
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
// FUNGSI KONEKSI MQTT
// =================================================

void hubungkanMQTT() {


  while (!client.connected()) {


    Serial.println("Menghubungkan MQTT...");


    String clientID = "ESP32Client-" + String(random(0xffff), HEX);



    if (client.connect(clientID.c_str())) {


      Serial.println("MQTT berhasil terhubung");



      // Subscribe topic perintah
      client.subscribe(topicPerintah);



      Serial.print("Subscribe topic: ");

      Serial.println(topicPerintah);


    }


    else {


      Serial.print("MQTT gagal, kode: ");

      Serial.println(client.state());


      delay(2000);


    }

  }


}



// =================================================
// SETUP
// =================================================

void setup() {


  // Memulai komunikasi Serial Monitor
  Serial.begin(115200);



  // Menghubungkan ESP32 ke WiFi
  hubungkanWiFi();



  // Konfigurasi PWM LED ESP32
  ledcAttachChannel(
    ledPin,
    pwmFrequency,
    pwmResolution,
    pwmChannel
  );



  // Kondisi awal LED mati
  ledcWrite(
    pwmChannel,
    0
  );



  // Mengatur broker MQTT
  client.setServer(
    mqttServer,
    mqttPort
  );



  // Menentukan fungsi callback
  client.setCallback(callback);


}



// =================================================
// LOOP
// =================================================

void loop() {


  // Mengecek koneksi MQTT
  if (!client.connected()) {


    hubungkanMQTT();


  }



  // Menjaga koneksi MQTT tetap aktif
  client.loop();


}
```
# 7. Hasil percobaan

Pada percobaan 4A, ESP32 berhasil melakukan koneksi ke broker MQTT dan menerima pesan JSON melalui mekanisme subscribe. Pengujian dilakukan dengan mengirimkan beberapa perintah melalui MQTT berupa data JSON dengan parameter `"perintah"` bernilai `"ON"` dan `"OFF"`.

Berdasarkan hasil pengujian, data JSON yang dikirim berhasil diterima oleh ESP32 dan dilakukan proses parsing dengan benar. Ketika menerima perintah `"ON"`, hasil parsing menunjukkan nilai ON dan LED memberikan respon menyala. Sedangkan ketika menerima perintah `"OFF"`, hasil parsing menunjukkan nilai OFF dan LED berubah menjadi mati.

Hasil percobaan menunjukkan bahwa proses subscribe MQTT, penerimaan data JSON, deserialisasi, dan pengendalian aktuator LED telah berjalan sesuai dengan tujuan percobaan. Perubahan kondisi LED dapat dilakukan secara langsung berdasarkan perintah yang dikirim melalui MQTT.
# 8. Analisis

Berdasarkan hasil pengujian, sistem komunikasi MQTT pada ESP32 berhasil berjalan dengan baik. ESP32 mampu melakukan subscribe pada topic yang telah ditentukan dan menerima data JSON yang dikirim melalui MQTT Explorer.

Data JSON yang diterima kemudian diproses menggunakan metode deserialisasi untuk mengambil nilai pada parameter `"perintah"`. Hasil parsing menunjukkan bahwa nilai perintah berhasil dibaca sesuai dengan data yang dikirim, yaitu `"ON"` dan `"OFF"`.

Ketika perintah `"ON"` diterima, ESP32 memberikan respon dengan mengaktifkan LED. Sebaliknya, ketika perintah `"OFF"` diterima, ESP32 berhasil menonaktifkan LED. Hal tersebut menunjukkan bahwa komunikasi antara MQTT broker, ESP32, dan aktuator berjalan sesuai dengan alur sistem yang dirancang.

Pada pengujian juga terlihat bahwa proses penerimaan pesan berlangsung secara langsung setelah data dikirimkan. Hal ini menunjukkan bahwa mekanisme subscribe MQTT dapat digunakan untuk memberikan kendali jarak jauh terhadap perangkat IoT secara real-time.

# 9. Kesimpulan

Berdasarkan percobaan yang telah dilakukan, dapat disimpulkan bahwa ESP32 berhasil menerapkan komunikasi MQTT dengan metode subscribe untuk menerima data JSON dari broker. Proses deserialisasi menggunakan ArduinoJson berhasil dilakukan sehingga nilai perintah dapat digunakan untuk mengendalikan aktuator LED.

Perintah yang dikirim dalam format JSON berupa `"ON"` dan `"OFF"` berhasil diproses dan menghasilkan perubahan kondisi LED sesuai instruksi. Dengan demikian, sistem subscribe MQTT dan pengolahan data JSON pada ESP32 telah berhasil diterapkan sesuai dengan tujuan Percobaan 4A.
# 10. foto percobaan
link: https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link
