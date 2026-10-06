# Modul 4 - Percobaan 4B
# Publish Data Sensor dan Subscribe Kendali Aktuator
## 1. Detail Percobaan
Percobaan ini membahas komunikasi data menggunakan protokol MQTT dengan metode publish dan subscribe pada ESP32. Pada percobaan ini ESP32 digunakan sebagai perangkat IoT yang mampu mengirimkan data sensor menuju broker MQTT serta menerima perintah dari broker untuk mengendalikan aktuator.
ESP32 terlebih dahulu melakukan koneksi ke jaringan WiFi TECNO POVA 6 kemudian terhubung dengan broker MQTT HiveMQ. Setelah koneksi berhasil dilakukan, ESP32 membaca data suhu dari sensor DHT11 dan mengirimkan data tersebut dalam bentuk format JSON menggunakan metode publish.
Selain melakukan publish data sensor, ESP32 juga melakukan subscribe pada topic perintah untuk menerima pesan dari MQTT Explorer. Pesan yang diterima kemudian diproses sehingga ESP32 dapat melakukan kendali terhadap LED berdasarkan perintah yang diberikan.
Pada percobaan ini diterapkan komunikasi dua arah, yaitu ESP32 dapat mengirimkan informasi sensor dan menerima perintah secara bersamaan.
## Spesifikasi yang Diharapkan
1. ESP32 berhasil terhubung dengan jaringan WiFi.
2. ESP32 berhasil terhubung dengan broker MQTT HiveMQ.
3. ESP32 berhasil membaca data suhu dari sensor DHT11.
4. ESP32 berhasil melakukan publish data sensor dalam format JSON.
5. Data sensor berhasil ditampilkan pada MQTT Explorer.
6. ESP32 berhasil melakukan subscribe pada topic perintah.
7. LED dapat dikendalikan berdasarkan pesan MQTT yang diterima.
8. Komunikasi publish dan subscribe dapat berjalan secara bersamaan.
## 2. Library / Dependencies
Library yang diperlukan:
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
```
### Fungsi Library:
``` text
| Library | Fungsi |
|---|---|
| `WiFi.h` | Digunakan untuk menghubungkan ESP32 dengan jaringan WiFi |
| `PubSubClient.h` | Digunakan untuk komunikasi MQTT publish dan subscribe |
| `ArduinoJson.h` | Digunakan untuk membuat dan melakukan parsing data JSON |
| `DHT.h` | Digunakan untuk membaca data suhu dari sensor DHT11 |
```
## Penjelasan Program
(Kode lengkap nanti dimasukkan di bagian ini, mengikuti kode yang kamu gunakan pada screenshot: DHT11 + publish JSON + subscribe LED)
Struktur program:
```text
Program dimulai
       |
       v
ESP32 terhubung WiFi
       |
       v
ESP32 terhubung MQTT Broker
       |
       +----------------+
       |                |
       v                v
 Membaca DHT11     Subscribe Topic
       |                |
       v                v
 Membuat JSON      Menerima JSON
       |                |
       v                v
 Publish MQTT     Parsing Perintah
       |                |
       +-------+--------+
               |
               v
        Kendali LED
```
## 4. Penjelasan Tiap Fungsi
```text
| No | Fungsi / Bagian Kode | Penjelasan |
|---|---|---|
| 1 | `WiFi.h` | Library untuk menghubungkan ESP32 dengan jaringan WiFi. |
| 2 | `PubSubClient.h` | Library untuk menjalankan komunikasi MQTT publish dan subscribe. |
| 3 | `ArduinoJson.h` | Library untuk membuat dan membaca data JSON. |
| 4 | `DHT.h` | Library untuk membaca data suhu dari sensor DHT11. |
| 5 | `DHT dht()` | Membuat objek sensor DHT yang digunakan untuk membaca nilai suhu. |
| 6 | `setup()` | Fungsi yang dijalankan satu kali saat ESP32 pertama aktif untuk melakukan konfigurasi awal. |
| 7 | `dht.begin()` | Mengaktifkan sensor DHT11 agar dapat digunakan membaca data suhu. |
| 8 | `client.setServer()` | Mengatur alamat broker MQTT yang digunakan. |
| 9 | `client.connect()` | Menghubungkan ESP32 dengan broker MQTT. |
| 10 | `client.publish()` | Mengirimkan data sensor dari ESP32 menuju broker MQTT. |
| 11 | `client.subscribe()` | Mendaftarkan ESP32 pada topic tertentu agar dapat menerima pesan MQTT. |
| 12 | `callback()` | Fungsi yang berjalan ketika ESP32 menerima pesan baru dari MQTT. |
| 13 | `deserializeJson()` | Mengubah data JSON yang diterima menjadi object yang dapat diproses ESP32. |
| 14 | `client.loop()` | Menjaga koneksi MQTT dan mengecek pesan baru yang masuk. |
| 15 | `millis()` | Digunakan untuk mengatur interval pengiriman data tanpa menghentikan program utama. |
```
## 5. Penjelasan Percabangan dan Conditional
```text
| No | Percabangan / Conditional | Penjelasan |
|---|---|---|
| 1 | `if (!client.connected())` | Mengecek apakah ESP32 masih terhubung dengan broker MQTT. Jika terputus, ESP32 melakukan koneksi ulang. |
| 2 | `if (error)` | Mengecek apakah proses parsing JSON berhasil dilakukan atau terdapat kesalahan. |
| 3 | `if (String(perintah) == "ON")` | Mengecek apakah perintah yang diterima adalah ON sehingga LED dinyalakan. |
| 4 | `else if (String(perintah) == "OFF")` | Mengecek apakah perintah yang diterima adalah OFF sehingga LED dimatikan. |
| 5 | `while (!client.connected())` | Melakukan percobaan koneksi ulang hingga ESP32 berhasil terhubung dengan MQTT broker. |
```
## 6. Jawaban Pertanyaan Praktikum
### 1. Jelaskan perbedaan komunikasi satu arah dan dua arah pada MQTT!
Jawaban:
Komunikasi satu arah pada MQTT hanya memungkinkan perangkat melakukan satu aktivitas, seperti hanya mengirimkan data menggunakan publish atau hanya menerima data menggunakan subscribe.
Sedangkan komunikasi dua arah memungkinkan perangkat melakukan publish dan subscribe secara bersamaan. Pada percobaan ini ESP32 dapat mengirimkan data suhu sensor melalui topic publish dan menerima perintah kendali LED melalui topic subscribe.
### 2. Mengapa diperlukan format JSON pada pengiriman data sensor?
Jawaban:
Format JSON digunakan karena memiliki struktur data yang sederhana dan mudah dibaca oleh perangkat IoT. Dengan menggunakan JSON, data sensor dapat dikirimkan dengan format yang lebih terstruktur sehingga mudah dilakukan proses parsing pada perangkat penerima.
### 3. Mengapa fungsi client.loop() harus berjalan terus menerus?
Jawaban:
Fungsi client.loop() digunakan untuk menjaga koneksi MQTT tetap aktif serta memeriksa apakah terdapat pesan baru yang masuk dari broker. Apabila fungsi ini tidak dijalankan secara berkala, ESP32 tidak dapat menerima pesan subscribe dan koneksi MQTT dapat terputus.
## 7. Hasil Percobaan
Pada percobaan 4B, ESP32 berhasil melakukan komunikasi MQTT dua arah dengan metode publish dan subscribe. ESP32 berhasil terhubung dengan WiFi dan broker MQTT HiveMQ.
Berdasarkan hasil pengujian, data suhu dari sensor DHT11 berhasil dibaca oleh ESP32 kemudian dikirimkan dalam format JSON melalui topic MQTT. Data tersebut berhasil diterima dan ditampilkan pada MQTT Explorer dengan format seperti:
```cpp
{
  "suhu":32.3
}
```
Selain melakukan publish data sensor, ESP32 juga berhasil menerima perintah melalui topic subscribe untuk melakukan kendali aktuator LED. Hal ini menunjukkan bahwa proses pengiriman data sensor dan penerimaan perintah dapat berjalan secara bersamaan.
## 8. Analisis
Berdasarkan hasil percobaan, sistem komunikasi MQTT dua arah pada ESP32 berhasil diterapkan. ESP32 mampu menjalankan fungsi publish untuk mengirimkan data suhu dari sensor DHT11 serta menjalankan fungsi subscribe untuk menerima perintah kendali.
Data suhu yang dikirimkan dalam format JSON berhasil diterima oleh broker MQTT dan ditampilkan pada MQTT Explorer. Hal tersebut menunjukkan bahwa proses komunikasi antara ESP32 dan broker berjalan dengan baik.
Selain itu, proses subscribe tetap berjalan ketika ESP32 melakukan pengiriman data sensor. Hal ini menunjukkan bahwa penggunaan MQTT memungkinkan perangkat IoT melakukan komunikasi dua arah secara real-time.
## 9. Kesimpulan
Berdasarkan percobaan 4B yang telah dilakukan, dapat disimpulkan bahwa ESP32 berhasil menerapkan komunikasi MQTT dua arah menggunakan metode publish dan subscribe.
ESP32 berhasil membaca data suhu dari sensor DHT11 kemudian mengirimkannya ke broker MQTT dalam format JSON. Selain itu, ESP32 juga berhasil menerima perintah melalui MQTT untuk melakukan kendali aktuator LED.
Dengan demikian, komunikasi dua arah pada sistem IoT berhasil diterapkan, dimana perangkat dapat mengirimkan data sensor sekaligus menerima instruksi kendali secara real-time.
## 10. Foto Percobaan
Link dokumentasi: https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link
