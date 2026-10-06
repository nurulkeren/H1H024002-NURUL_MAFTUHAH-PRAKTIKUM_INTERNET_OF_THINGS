# Praktikum Internet of Things

## Pertemuan 4A
# Subscribe dan Deserialisasi Data JSON untuk Kendali Aktuator


## Identitas

Nama : Nurul Maftuhah  
NIM : H1H024002  


## Deskripsi Percobaan

Pada percobaan 4A dilakukan implementasi komunikasi MQTT dengan metode
subscribe untuk menerima perintah kendali dari broker MQTT.

Pada percobaan ini ESP32 digunakan untuk menerima pesan dalam format JSON,
kemudian melakukan proses deserialisasi data untuk mengambil nilai perintah.
Nilai perintah tersebut digunakan untuk mengendalikan aktuator berupa LED.

Sistem yang dibuat memungkinkan perangkat IoT menerima instruksi secara
real-time melalui jaringan internet.


## Tujuan Percobaan

1. Memahami mekanisme komunikasi MQTT menggunakan metode subscribe.
2. Mengimplementasikan penerimaan data JSON pada ESP32.
3. Memahami proses deserialisasi data menggunakan ArduinoJson.
4. Mengontrol aktuator LED berdasarkan perintah yang diterima.




## Konfigurasi Jaringan WiFi

```cpp
const char* ssid = "TECNO POVA 6";
const char* password = "hurufbesar";
