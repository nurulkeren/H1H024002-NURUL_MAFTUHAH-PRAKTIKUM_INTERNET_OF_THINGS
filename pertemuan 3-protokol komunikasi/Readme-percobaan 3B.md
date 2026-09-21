# Modul 3 - Percobaan 3B
# Komunikasi Data Menggunakan MQTT

## 1. Detail Percobaan

Percobaan ini membahas komunikasi data antara ESP32 dan broker MQTT menggunakan pola **publish-subscribe**. Data sensor dikirimkan dalam format JSON melalui topic MQTT.

Pada percobaan ini digunakan broker MQTT publik `broker.hivemq.com` dengan port `1883`. ESP32 berperan sebagai publisher yang mengirimkan data ke topic tertentu. Data tersebut kemudian dapat diterima dan dilihat menggunakan aplikasi client MQTT yang melakukan subscribe pada topic yang sama.

### Spesifikasi yang Diharapkan

1. ESP32 berhasil terhubung ke broker MQTT publik.
2. ESP32 berhasil melakukan publish data sensor dalam format JSON.
3. Data dipublikasikan secara berkala ke suatu topic.
4. Data dapat diverifikasi menggunakan aplikasi MQTT client.
5. Serial Monitor menampilkan status koneksi dan data yang berhasil dipublikasikan.

---

## 2. Library / Dependencies

Library yang diperlukan:

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
```

### Fungsi Library

| Library | Fungsi |
|---|---|
| `WiFi.h` | Mengatur koneksi ESP32 ke jaringan WiFi |
| `PubSubClient.h` | Digunakan untuk komunikasi MQTT |
| `ArduinoJson.h` | Membuat data dalam format JSON |

---

## 3. Penjelasan Code

```cpp
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
```

---

## 4. Penjelasan Setiap Fungsi

| Fungsi | Penjelasan |
|---|---|
| `WiFi.begin()` | Menghubungkan ESP32 ke jaringan WiFi |
| `WiFi.status()` | Mengecek status koneksi WiFi |
| `PubSubClient client()` | Membuat client MQTT |
| `client.setServer()` | Menentukan broker dan port MQTT |
| `client.connect()` | Menghubungkan ESP32 ke broker MQTT |
| `client.connected()` | Mengecek apakah client masih terhubung |
| `client.loop()` | Menjaga komunikasi MQTT tetap berjalan |
| `client.publish()` | Mengirim data ke topic MQTT |
| `client.state()` | Mengetahui status/kode koneksi MQTT |
| `serializeJson()` | Mengubah JSON menjadi teks |
| `delay()` | Memberikan jeda waktu |

---

## 5. Penjelasan Percabangan / Conditional

Program menggunakan percabangan untuk mengecek koneksi MQTT.

```cpp
if (!client.connected()) {
    hubungkanMQTT();
}
```

Jika ESP32 tidak terhubung ke broker MQTT, fungsi `hubungkanMQTT()` akan dipanggil untuk melakukan koneksi kembali.

Program juga menggunakan percabangan ketika melakukan koneksi MQTT:

```cpp
if (client.connect(clientId.c_str())) {
    Serial.println("berhasil terhubung!");
} else {
    Serial.print("gagal, rc=");
    Serial.print(client.state());
}
```

Jika koneksi berhasil, Serial Monitor menampilkan pesan berhasil. Jika gagal, program menampilkan kode status koneksi dan mencoba kembali setelah 2 detik.

---

## 6. Penjelasan Perulangan

Program menggunakan perulangan `while` untuk menunggu koneksi WiFi.

```cpp
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
```

ESP32 akan terus menunggu sampai berhasil terhubung ke WiFi.

Selain itu, terdapat `while` pada fungsi `hubungkanMQTT()`:

```cpp
while (!client.connected()) {
    ...
}
```

Perulangan ini membuat ESP32 terus mencoba terhubung ke broker MQTT sampai koneksi berhasil.

Fungsi `loop()` Arduino juga berjalan terus-menerus untuk menjaga proses komunikasi MQTT dan melakukan publish data secara berkala.

---

## 7. Diagram Alur / Flowchart

```text
        +----------------+
        |     Mulai      |
        +-------+--------+
                |
                v
       +-------------------+
       | Inisialisasi Serial|
       +---------+---------+
                 |
                 v
       +-------------------+
       | Hubungkan ke WiFi |
       +---------+---------+
                 |
                 v
        /------------------\
       / Berhasil terhubung? \
       \                    /
        \------------------/
           |            |
         Tidak         Ya
           |            |
           v            v
      +---------+  +------------------+
      | Tunggu  |  | Atur Server MQTT |
      | 500 ms  |  +--------+---------+
      +----+----+           |
           |                v
           +-------> +----------------+
                     | Cek MQTT       |
                     +-------+--------+
                             |
                             v
                    /----------------\
                   / Terhubung MQTT?  \
                   \                  /
                    \----------------/
                       |          |
                     Tidak       Ya
                       |          |
                       v          v
                +-----------+  +-------------+
                | Hubungkan |  | client.loop |
                | kembali   |  +------+------+
                +-----+-----+         |
                      |               v
                      +------> +-------------+
                               | Buat JSON   |
                               | Sensor      |
                               +------+------+ 
                                      |
                                      v
                               +-------------+
                               | Publish ke  |
                               | Topic MQTT  |
                               +------+------+
                                      |
                                      v
                               +-------------+
                               | Tunggu 5 s  |
                               +------+------+
                                      |
                                      v
                                   Ulangi
```

---

## 8. Jawaban Pertanyaan Praktikum

### 1. Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik?

Topic digunakan sebagai alamat atau jalur untuk mengelompokkan data dalam komunikasi MQTT.

Publisher mengirim data ke topic tertentu, sedangkan subscriber menerima data dengan melakukan subscribe pada topic tersebut.

Topic perlu dibuat unik agar data dari satu kelompok tidak tercampur dengan data dari kelompok lain. Pada modul juga disarankan menggunakan nama topic yang menyertakan nama kelompok.

Contoh:

```text
unsoed/tk245004/kelompok10/sensor
```

### 2. Jelaskan fungsi dari perintah `client.loop()` yang dipanggil pada setiap iterasi `loop()`!

`client.loop()` digunakan untuk menjaga komunikasi MQTT tetap berjalan. Perintah ini dipanggil pada setiap iterasi fungsi `loop()` agar koneksi MQTT tetap diproses oleh client.

### 3. Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program berjalan?

Program akan mendeteksi bahwa client sudah tidak terhubung melalui:

```cpp
if (!client.connected()) {
    hubungkanMQTT();
}
```

Kemudian fungsi `hubungkanMQTT()` akan dijalankan untuk mencoba menghubungkan ESP32 kembali ke broker MQTT.

Program akan terus mencoba koneksi sampai berhasil.

---

## 9. Hasil Percobaan

Berdasarkan hasil pengujian, ESP32 berhasil terhubung ke broker MQTT dan dapat melakukan publish data JSON ke topic:

```text
unsoed/tk245004/kelompok1/sensor
```

Pengiriman dilakukan setiap 10 detik dari detik 0 sampai detik 90 dengan total 10 kali pengiriman. Data JSON yang dipublish adalah:

```json
{"suhu":28.5,"kelembaban":65}
```

Pada setiap pengiriman, status broker menunjukkan **Terhubung**, data berhasil dipublish, dan data yang sama berhasil diterima oleh subscriber melalui MQTT client. 

| No. | Waktu (s) | Status Broker | Topic | Data JSON | Diterima Subscriber | Status |
|---|---:|---|---|---|---|---|
| 1 | 0 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 2 | 10 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 3 | 20 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 4 | 30 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 5 | 40 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 6 | 50 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 7 | 60 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 8 | 70 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 9 | 80 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |
| 10 | 90 | Terhubung | `unsoed/tk245004/kelompok1/sensor` | `{"suhu":28.5,"kelembaban":65}` | Ya | Berhasil |

## 10. Analisis

Berdasarkan hasil percobaan, komunikasi menggunakan protokol MQTT berjalan dengan baik. ESP32 berhasil terhubung ke broker MQTT dan melakukan publish data JSON ke topic yang telah ditentukan.

Data yang dipublish oleh ESP32 berhasil diterima oleh subscriber pada MQTT client. Selama pengujian dari detik 0 sampai 90, status broker tetap **Terhubung** dan seluruh data yang dikirim berhasil diterima oleh subscriber.

Hasil tersebut menunjukkan bahwa mekanisme publish-subscribe pada MQTT dapat berjalan sesuai dengan spesifikasi percobaan. Topic yang digunakan juga dapat menjadi jalur komunikasi antara publisher dan subscriber.

## 11. Kesimpulan

Percobaan komunikasi data menggunakan MQTT berhasil dilakukan. ESP32 dapat terhubung ke broker MQTT dan mempublikasikan data JSON ke topic `unsoed/tk245004/kelompok1/sensor`.

Seluruh data yang dipublish berhasil diterima oleh subscriber, sehingga proses komunikasi menggunakan MQTT berjalan sesuai dengan yang diharapkan.

## 12. foto percobaan
link:  https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link
