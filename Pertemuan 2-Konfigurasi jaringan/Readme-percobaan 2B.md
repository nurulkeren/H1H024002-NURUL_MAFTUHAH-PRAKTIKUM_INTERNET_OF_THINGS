# Modul 2 Konfigurasi Jaringan

## 1. Penjelasan Singkat Mengenai Detail Percobaan

Pada percobaan ini, ESP32 dikonfigurasi sebagai **Access Point (AP)**
sehingga ESP32 dapat membuat jaringan WiFi sendiri.

Access Point yang dibuat menggunakan SSID `ESP32_AccessPoint` dengan
password `12345678`. Perangkat lain kemudian dihubungkan ke jaringan
yang dibuat oleh ESP32.

Pada pengujian, jumlah perangkat yang terhubung pada awalnya adalah
`0`. Setelah perangkat dengan nama `TECNOPOVA6` terhubung ke Access
Point `ESP32_AccessPoint`, jumlah client berubah menjadi `1`. Hasil
pengamatan tersebut ditampilkan melalui Serial Monitor.

## 2. Library / Dependencies

Library yang digunakan pada percobaan ini adalah:

```cpp
#include <WiFi.h>
```
Library WiFi.h digunakan untuk mengatur dan mengelola fungsi jaringan
WiFi pada ESP32, termasuk membuat Access Point dan mengetahui jumlah
perangkat yang terhubung.

## 3. Penjelasan kode
```cpp
#include <WiFi.h>  // Memanggil library WiFi untuk mengatur jaringan WiFi ESP32

// Menentukan nama Access Point yang akan dibuat oleh ESP32
const char* ap_ssid = "ESP32_AccessPoint";

// Menentukan password Access Point
// Password minimal terdiri dari 8 karakter
const char* ap_password = "12345678";

void setup() {
  // Memulai komunikasi dengan Serial Monitor
  Serial.begin(115200);

  // Mengatur ESP32 agar bekerja sebagai Access Point
  WiFi.mode(WIFI_AP);

  // Membuat jaringan WiFi menggunakan SSID dan password yang telah ditentukan
  WiFi.softAP(ap_ssid, ap_password);

  // Mendapatkan IP Address dari Access Point
  IPAddress apIP = WiFi.softAPIP();

  // Menampilkan informasi bahwa Access Point telah aktif
  Serial.println("Access Point aktif!");

  // Menampilkan nama atau SSID Access Point
  Serial.print("SSID : ");
  Serial.println(ap_ssid);

  // Menampilkan IP Address Access Point
  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {
  // Menghitung jumlah perangkat yang sedang terhubung
  // ke Access Point ESP32
  int jumlahClient = WiFi.softAPgetStationNum();

  // Menampilkan jumlah perangkat yang terhubung
  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  // Melakukan pengecekan jumlah client setiap 5 detik
  delay(5000);
}
```
## 4. Penjelasan Fungsi
| Fungsi                       | Keterangan                                                  |
| ---------------------------- | ----------------------------------------------------------- |
| `Serial.begin(115200)`       | Memulai komunikasi antara ESP32 dan Serial Monitor.         |
| `WiFi.mode(WIFI_AP)`         | Mengatur ESP32 agar bekerja sebagai Access Point.           |
| `WiFi.softAP()`              | Membuat jaringan WiFi menggunakan ESP32.                    |
| `WiFi.softAPIP()`            | Mendapatkan IP Address Access Point.                        |
| `WiFi.softAPgetStationNum()` | Mengetahui jumlah perangkat yang terhubung ke Access Point. |
| `Serial.print()`             | Menampilkan informasi pada Serial Monitor.                  |
| `delay()`                    | Memberikan jeda waktu selama 5 detik.                       |

## 5. Penjelasan Percabangan / Conditional
Pada program Percobaan 2B tidak terdapat percabangan if-else.
Program langsung menjalankan konfigurasi Access Point pada fungsi
setup().

Pada fungsi loop(), program mengambil jumlah perangkat yang
terhubung menggunakan:
```cpp
int jumlahClient = WiFi.softAPgetStationNum();
```
Nilai jumlah client kemudian ditampilkan pada Serial Monitor setiap
5 detik.

## 6. Jawaban Pertanyaan Praktikum
### 1. Mengapa alamat IP default Access Point pada ESP32 umumnya bernilai 192.168.4.1?
Alamat IP 192.168.4.1 merupakan alamat IP default yang digunakan ESP32 ketika berfungsi sebagai Access Point. Alamat tersebut menjadi alamat ESP32 dalam jaringan lokal yang dibuatnya, sehingga perangkat lain yang terhubung ke Access Point dapat berkomunikasi dengan ESP32. Pada modul, IP Address default Access Point ESP32 ditentukan sebagai 192.168.4.1
### 2. Apa perbedaan mendasar antara mode Station dan mode Access Point pada ESP32?
Pada mode Station (STA), ESP32 berperan sebagai klien yang terhubung ke jaringan WiFi yang sudah tersedia, seperti router atau hotspot. Sedangkan pada mode Access Point (AP), ESP32 berperan sebagai penyedia jaringan WiFi yang dapat ditemukan dan digunakan oleh perangkat lain
### 3. Jelaskan risiko keamanan apabila password Access Point tidak diberikan atau terlalu sederhana!
Jika Access Point tidak menggunakan password atau menggunakan password yang terlalu sederhana, jaringan akan lebih mudah diakses oleh perangkat yang tidak berwenang. Hal tersebut dapat menyebabkan perangkat yang tidak dikenal terhubung ke jaringan dan berpotensi mengakses komunikasi atau layanan yang tersedia pada ESP32. Oleh karena itu, Access Point sebaiknya menggunakan password yang cukup kuat dan tidak mudah ditebak
### 4. Modifikasi program agar ESP32 berjalan pada mode AP+STA (terhubung ke WiFi rumah sekaligus menyediakan Access Point), dan berikan penjelasan di setiap baris kode
```cpp
#include <WiFi.h>  // Memanggil library WiFi untuk mengatur koneksi WiFi ESP32

// Data WiFi yang akan digunakan ESP32 sebagai Station
const char* sta_ssid = "TECNOPOVA6";
const char* sta_password = "hurufbesar";

// Data WiFi yang akan dibuat oleh ESP32 sebagai Access Point
const char* ap_ssid = "ESP32_AccessPoint";
const char* ap_password = "12345678";

void setup() {
  // Memulai komunikasi Serial Monitor dengan baud rate 115200
  Serial.begin(115200);

  // Mengatur ESP32 agar bekerja dalam mode AP + STA
  // ESP32 dapat terhubung ke WiFi sekaligus membuat Access Point
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan ESP32 ke jaringan WiFi sebagai Station
  WiFi.begin(sta_ssid, sta_password);

  // Menunggu sampai ESP32 berhasil terhubung ke WiFi Station
  Serial.print("Menghubungkan ke WiFi Station");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Menampilkan informasi bahwa koneksi Station berhasil
  Serial.println();
  Serial.println("WiFi Station berhasil terhubung");

  // Menampilkan IP Address ESP32 sebagai Station
  Serial.print("IP Station : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point pada ESP32
  WiFi.softAP(ap_ssid, ap_password);

  // Mendapatkan IP Address dari Access Point
  IPAddress apIP = WiFi.softAPIP();

  // Menampilkan informasi Access Point
  Serial.println("Access Point aktif!");

  // Menampilkan SSID Access Point
  Serial.print("SSID AP : ");
  Serial.println(ap_ssid);

  // Menampilkan IP Address Access Point
  Serial.print("IP AP : ");
  Serial.println(apIP);
}

void loop() {
  // Mengecek jumlah perangkat yang terhubung
  // ke Access Point ESP32
  int jumlahClient = WiFi.softAPgetStationNum();

  // Menampilkan jumlah perangkat yang terhubung
  Serial.print("Jumlah perangkat terhubung ke AP: ");
  Serial.println(jumlahClient);

  // Mengecek status koneksi Station
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status Station: Terhubung");
  } else {
    Serial.println("Status Station: Terputus");
  }

  // Melakukan pengecekan setiap 5 detik
  delay(5000);
}
```
## 7. penjelasan detail percobaan
Pada percobaan 2B, ESP32 dikonfigurasi sebagai Access Point (AP) untuk membuat jaringan WiFi sendiri. ESP32 menggunakan SSID ESP32_AccessPoint dan password 12345678. Setelah Access Point aktif, perangkat lain dapat mencari dan terhubung ke jaringan WiFi yang dibuat oleh ESP32.
Pengujian dilakukan dengan mengamati IP Address Access Point dan jumlah perangkat (client) yang terhubung melalui Serial Monitor. Pada awal pengujian, jumlah client adalah 0 karena belum ada perangkat yang terhubung. Setelah perangkat dengan nama TECNOPOVA6 terhubung ke jaringan ESP32_AccessPoint, jumlah client berubah menjadi 1.
Percobaan ini dilakukan menggunakan rangkaian yang sama seperti percobaan 2A, sehingga tidak ada perubahan pada rangkaian fisik. Fokus percobaan adalah pada konfigurasi ESP32 sebagai Access Point dan pengamatan koneksi perangkat melalui Serial Monitor.

## 8. foto percobaan
link: https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link




