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

