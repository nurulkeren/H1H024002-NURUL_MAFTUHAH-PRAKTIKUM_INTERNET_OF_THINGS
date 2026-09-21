# Modul 3 - Percobaan 3A
# Komunikasi Data Menggunakan HTTP

## 1. Detail Percobaan

Percobaan ini membahas komunikasi data antara ESP32 dan server menggunakan protokol HTTP dengan metode POST. Data yang dikirimkan oleh ESP32 menggunakan format JSON. Pada percobaan ini digunakan endpoint `https://httpbin.org/post` sebagai server pengujian.

ESP32 terlebih dahulu terhubung ke jaringan WiFi. Setelah berhasil terhubung, ESP32 membuat data suhu dan kelembaban dalam format JSON, kemudian mengirimkannya ke server menggunakan HTTP POST. Hasil pengiriman dapat dilihat melalui Serial Monitor berupa kode response HTTP dan isi response dari server.

### Spesifikasi yang Diharapkan

1. ESP32 berhasil terhubung ke jaringan WiFi.
2. ESP32 berhasil mengirim data suhu dan kelembaban dalam format JSON.
3. Data dikirim menggunakan metode HTTP POST.
4. Serial Monitor menampilkan kode response HTTP dan isi response dari server.
5. Tidak terjadi error saat program dikompilasi maupun saat pengiriman data.

---

## 2. Library / Dependencies

Library yang diperlukan:

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
```

### Fungsi Library

| Library | Fungsi |
|---|---|
| `WiFi.h` | Mengatur koneksi ESP32 ke jaringan WiFi |
| `HTTPClient.h` | Digunakan untuk melakukan komunikasi HTTP |
| `ArduinoJson.h` | Membuat dan mengolah data dalam format JSON |

---

## 3. Penjelasan Code

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Menentukan nama jaringan WiFi dan password
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

// Menentukan alamat server yang digunakan untuk pengujian HTTP POST
const char* serverUrl = "https://httpbin.org/post";

void setup() {
  // Memulai komunikasi Serial Monitor dengan baud rate 115200
  Serial.begin(115200);

  // Memulai koneksi ESP32 ke jaringan WiFi
  WiFi.begin(ssid, password);

  // Menampilkan pesan proses koneksi
  Serial.print("Menghubungkan ke WiFi");

  // Menunggu sampai ESP32 berhasil terhubung ke WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Menampilkan informasi bahwa WiFi berhasil terhubung
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
}

void loop() {
  // Mengecek apakah ESP32 masih terhubung ke WiFi
  if (WiFi.status() == WL_CONNECTED) {

    // Membuat objek HTTP
    HTTPClient http;

    // Menentukan alamat server yang akan dituju
    http.begin(serverUrl);

    // Menentukan bahwa data yang dikirim memiliki format JSON
    http.addHeader("Content-Type", "application/json");

    // Membuat objek data sensor dalam format JSON
    JsonDocument doc;

    // Menambahkan data suhu ke dalam JSON
    doc["suhu"] = 28.5;

    // Menambahkan data kelembaban ke dalam JSON
    doc["kelembaban"] = 65.0;

    // Membuat variabel untuk menyimpan JSON dalam bentuk string
    String requestBody;

    // Mengubah objek JSON menjadi string
    serializeJson(doc, requestBody);

    // Menampilkan data yang akan dikirim
    Serial.print("Mengirim data: ");
    Serial.println(requestBody);

    // Mengirim data menggunakan metode HTTP POST
    int httpResponseCode = http.POST(requestBody);

    // Mengecek apakah pengiriman berhasil
    if (httpResponseCode > 0) {

      // Menampilkan kode response HTTP
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);

      // Menampilkan isi response dari server
      Serial.println("Isi Response:");
      Serial.println(http.getString());

    } else {

      // Menampilkan kode error jika pengiriman gagal
      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode);
    }

    // Mengakhiri koneksi HTTP
    http.end();
  }

  // Memberikan jeda 10 detik sebelum mengirim data berikutnya
  delay(10000);
}
```

---

## 4. Penjelasan Setiap Fungsi

| Fungsi | Penjelasan |
|---|---|
| `Serial.begin(115200)` | Memulai komunikasi dengan Serial Monitor |
| `WiFi.begin()` | Memulai koneksi ESP32 ke jaringan WiFi |
| `WiFi.status()` | Mengecek status koneksi WiFi |
| `HTTPClient http` | Membuat objek untuk komunikasi HTTP |
| `http.begin()` | Menentukan alamat server yang akan digunakan |
| `http.addHeader()` | Menambahkan informasi header pada request HTTP |
| `JsonDocument` | Membuat objek untuk menyimpan data JSON |
| `doc["key"] = value` | Menambahkan data key-value ke JSON |
| `serializeJson()` | Mengubah objek JSON menjadi string |
| `http.POST()` | Mengirim data ke server menggunakan metode POST |
| `http.getString()` | Mengambil isi response dari server |
| `http.end()` | Mengakhiri komunikasi HTTP |
| `delay()` | Memberikan jeda waktu |

---

## 5. Penjelasan Percabangan / Conditional

Program menggunakan percabangan `if-else` untuk mengecek status koneksi WiFi.

```cpp
if (WiFi.status() == WL_CONNECTED) {
    // Proses pengiriman data
}
```

Jika ESP32 masih terhubung ke WiFi, maka proses komunikasi HTTP akan dijalankan.

Program juga menggunakan percabangan untuk mengecek hasil pengiriman HTTP.

```cpp
if (httpResponseCode > 0) {
    Serial.print("Kode Response HTTP: ");
    Serial.println(httpResponseCode);
} else {
    Serial.print("Pengiriman gagal, kode error: ");
    Serial.println(httpResponseCode);
}
```

Jika nilai `httpResponseCode` lebih dari 0, berarti ESP32 mendapatkan response dari server. Jika tidak, program menampilkan kode error pengiriman.

---

## 6. Penjelasan Perulangan

Program menggunakan perulangan `while` pada proses koneksi WiFi.

```cpp
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
```

Perulangan tersebut membuat ESP32 terus menunggu sampai berhasil terhubung ke jaringan WiFi.

Selain itu, fungsi `loop()` akan dijalankan berulang kali sehingga proses pengiriman data HTTP dapat dilakukan secara berkala.

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
      +---------+  +----------------+
      | Tunggu  |  | Buat koneksi   |
      | 500 ms  |  | HTTP           |
      +----+----+  +-------+--------+
           |               |
           +-------<-------+
                           |
                           v
                  +------------------+
                  | Buat data JSON   |
                  | suhu & kelembaban|
                  +--------+---------+
                           |
                           v
                  +------------------+
                  | HTTP POST        |
                  | ke server        |
                  +--------+---------+
                           |
                           v
                  +------------------+
                  | Cek response HTTP|
                  +--------+---------+
                           |
                    +------+------+
                    |             |
                  Berhasil       Gagal
                    |             |
                    v             v
             +-------------+  +----------+
             | Tampilkan   |  | Tampilkan|
             | response    |  | error    |
             +------+------+  +----+-----+
                    |              |
                    +------+-------+
                           |
                           v
                    +-------------+
                    | Tunggu 10 s |
                    +------+------+
                           |
                           v
                       Ulangi
```

---

## 8. Jawaban Pertanyaan Praktikum

### 1. Gambarkan diagram alur proses pengiriman data melalui HTTP POST pada program di atas!

Diagram alur proses pengiriman data HTTP POST dapat dilihat pada bagian **7. Diagram Alur / Flowchart**.

### 2. Apa fungsi dari perintah `http.addHeader("Content-Type", "application/json")`?

Perintah tersebut digunakan untuk memberikan informasi kepada server bahwa data yang dikirim oleh ESP32 memiliki format `application/json`.

Dengan demikian, server dapat mengetahui bahwa isi request yang dikirim berupa data JSON.

### 3. Jelaskan arti dari kode response HTTP 200 dan sebutkan salah satu contoh kode response HTTP lain beserta artinya!

Kode response HTTP `200` menunjukkan bahwa request yang dikirim berhasil diproses oleh server.

Contoh kode lainnya adalah:

- `404` → resource atau halaman yang diminta tidak ditemukan.
- `500` → terjadi kesalahan pada server.

### 4. Modifikasi program agar ESP32 dapat mengirimkan data tambahan berupa waktu menggunakan `millis()`.

Tambahkan data berikut setelah data suhu dan kelembaban:

```cpp
// Menambahkan waktu sejak ESP32 dinyalakan
doc["waktu"] = millis();
```

Baris tersebut mengambil nilai waktu dalam milidetik sejak ESP32 mulai dijalankan dan memasukkannya ke dalam objek JSON dengan key `waktu`.

Contoh JSON yang dikirim:

```json
{
  "suhu": 28.5,
  "kelembaban": 65.0,
  "waktu": 12345
}
```

---

## 9. Hasil Percobaan

Berdasarkan hasil pengujian, ESP32 berhasil terhubung ke WiFi dan dapat mengirimkan data JSON menggunakan metode HTTP POST ke server `httpbin.org`.

Pengiriman data dilakukan setiap 10 detik, mulai dari detik 0 sampai detik 90, sehingga terdapat 10 kali pengiriman. Pada setiap pengiriman, data JSON yang dikirim adalah:

```json
{"suhu":28.5,"kelembaban":65}
```

Seluruh pengiriman mendapatkan **HTTP Response Code 200** dan response body dari server menunjukkan data JSON yang telah dikirim. Dengan demikian, seluruh proses pengiriman data pada percobaan berhasil dilakukan.

| No. | Waktu (s) | Data JSON | Response Code | Status |
|---|---:|---|---:|---|
| 1 | 0 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 2 | 10 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 3 | 20 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 4 | 30 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 5 | 40 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 6 | 50 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 7 | 60 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 8 | 70 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 9 | 80 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |
| 10 | 90 | `{"suhu":28.5,"kelembaban":65}` | 200 | Berhasil |

## 10. Analisis

Berdasarkan hasil percobaan, komunikasi antara ESP32 dengan server melalui protokol HTTP berjalan dengan baik. Data suhu dan kelembaban berhasil dikirim dalam format JSON setiap 10 detik.

Setiap pengiriman mendapatkan response code **200**, yang menunjukkan bahwa request berhasil diterima dan diproses oleh server. Response body juga menampilkan kembali data JSON yang dikirim sehingga data yang dikirim oleh ESP32 dapat diverifikasi.

Selama 10 kali pengiriman dari detik 0 sampai 90, tidak terdapat pengiriman yang gagal. Hal ini menunjukkan bahwa program HTTP POST dapat berjalan sesuai dengan spesifikasi percobaan.

## 11. Kesimpulan

Percobaan komunikasi data menggunakan HTTP berhasil dilakukan. ESP32 dapat mengirimkan data JSON berupa suhu dan kelembaban ke server menggunakan metode HTTP POST secara berkala setiap 10 detik. Seluruh pengiriman mendapatkan response code 200 dan data dapat diterima oleh server dengan baik.
