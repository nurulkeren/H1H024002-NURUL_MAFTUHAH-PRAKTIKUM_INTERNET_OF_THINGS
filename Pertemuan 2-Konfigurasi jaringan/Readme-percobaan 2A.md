# Praktikum WiFi ESP32

## 1. Detail Percobaan

Praktikum ini membahas penggunaan jaringan WiFi pada ESP32. 
Percobaan yang dilakukan meliputi konfigurasi ESP32 sebagai 
Station (STA), Access Point (AP), dan modifikasi ESP32 agar 
dapat bekerja dalam mode AP+STA.

Pada mode Station, ESP32 dihubungkan ke jaringan WiFi 
TECNOPOVA6 menggunakan password hurufbesar. Pada mode 
Access Point, ESP32 membuat jaringan WiFi sendiri dengan 
SSID ESP32_AccessPoint. Seluruh hasil pengujian diamati 
melalui Serial Monitor.

---

## 2. Library / Dependencies

Library yang diperlukan:

```cpp
#include <WiFi.h>
```
## 3. penjelasan code
```cpp
#include <WiFi.h>  // Memanggil library WiFi untuk mengatur koneksi WiFi pada ESP32

// Menentukan nama WiFi (SSID) dan password yang akan digunakan
const char* ssid = "TECNOPOVA6";
const char* password = "hurufbesar";

// Menentukan GPIO 2 sebagai pin untuk LED indikator koneksi
const int ledPin = 2;

void setup() {
  // Memulai komunikasi Serial Monitor dengan baud rate 115200
  Serial.begin(115200);

  // Mengatur GPIO 2 sebagai output untuk mengontrol LED
  pinMode(ledPin, OUTPUT);

  // Memastikan LED dalam kondisi mati saat awal program dijalankan
  digitalWrite(ledPin, LOW);

  // Mengatur ESP32 agar bekerja sebagai Station (STA)
  // ESP32 berperan sebagai perangkat yang terhubung ke WiFi
  WiFi.mode(WIFI_STA);

  // Memulai proses koneksi ESP32 ke WiFi menggunakan SSID dan password
  WiFi.begin(ssid, password);

  // Menampilkan pesan bahwa ESP32 sedang mencoba terhubung ke WiFi
  Serial.print("Menghubungkan ke WiFi");

  // Menunggu sampai ESP32 berhasil terhubung ke jaringan WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  // Menunggu 500 milidetik sebelum mengecek kembali
    Serial.print(".");  // Menampilkan titik sebagai tanda proses koneksi
  }

  // Bagian ini dijalankan setelah ESP32 berhasil terhubung ke WiFi
  Serial.println();
  Serial.println("WiFi berhasil terhubung");

  // Menampilkan IP Address yang diperoleh ESP32 dari jaringan WiFi
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  // Menampilkan MAC Address ESP32
  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  // Menampilkan kekuatan sinyal WiFi dalam satuan dBm
  Serial.print("RSSI (dBm) : ");
  Serial.println(WiFi.RSSI());

  // Menyalakan LED sebagai indikator bahwa ESP32 berhasil terhubung ke WiFi
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // Mengecek status koneksi WiFi setiap kali fungsi loop dijalankan
  if (WiFi.status() == WL_CONNECTED) {

    // Jika masih terhubung, tampilkan status "Terhubung"
    Serial.println("Status: Terhubung");

  } else {

    // Jika koneksi terputus, tampilkan status "Terputus"
    Serial.println("Status: Terputus");

    // Mematikan LED sebagai tanda bahwa koneksi WiFi terputus
    digitalWrite(ledPin, LOW);
  }

  // Memberikan jeda 5 detik sebelum melakukan pengecekan berikutnya
  delay(5000);
}

```
Program digunakan untuk menghubungkan ESP32 ke jaringan WiFi
sebagai Station. Setelah berhasil terhubung, program menampilkan
IP Address, MAC Address, dan RSSI pada Serial Monitor. LED pada
GPIO 2 digunakan sebagai indikator koneksi.
## 4. Penjelasan setiap fungsi
| Fungsi                 | Penjelasan                               |
| ---------------------- | ---------------------------------------- |
| `Serial.begin(115200)` | Memulai komunikasi dengan Serial Monitor |
| `pinMode()`            | Mengatur mode pin GPIO                   |
| `digitalWrite()`       | Mengatur kondisi LED                     |
| `WiFi.mode(WIFI_STA)`  | Mengatur ESP32 sebagai Station           |
| `WiFi.begin()`         | Memulai koneksi ke jaringan WiFi         |
| `WiFi.status()`        | Mengecek status koneksi WiFi             |
| `WiFi.localIP()`       | Mendapatkan IP Address ESP32             |
| `WiFi.macAddress()`    | Mendapatkan MAC Address ESP32            |
| `WiFi.RSSI()`          | Mendapatkan kekuatan sinyal WiFi         |
| `delay()`              | Memberikan jeda waktu                    |
## 5. Penjelasan Percabangan / Conditional
Program menggunakan percabangan if-else untuk mengecek
status koneksi WiFi.
```cpp
if (WiFi.status() == WL_CONNECTED) {
  Serial.println("Status: Terhubung");
} else {
  Serial.println("Status: Terputus");
  digitalWrite(ledPin, LOW);
}
```
Jika status WiFi adalah WL_CONNECTED, maka ESP32 menampilkan
status "Terhubung". Jika tidak, ESP32 menampilkan status
"Terputus" dan LED dimatikan.

Program juga menggunakan perulangan while pada proses awal
koneksi:
```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```
Perulangan tersebut membuat ESP32 terus menunggu sampai berhasil
terhubung ke jaringan WiFi.
## 6. Jawaban Pertanyaan Praktikum 2A
1. Gambarkan diagram alur (flowchart) proses koneksi ESP32 ke jaringan WiFi pada program di atas!<br>
   
   Mulai
  ↓
Inisialisasi Serial dan LED
  ↓
LED OFF
  ↓
Atur WiFi menjadi STA
  ↓
Hubungkan ke WiFi
  ↓
Berhasil terhubung?
  ├── Tidak → Tunggu 500 ms → Cek kembali
  │
  └── Ya → Tampilkan IP, MAC, dan RSSI
              ↓
           LED ON
              ↓
        Cek status WiFi
              ↓
         Tunggu 5 detik
              ↓
            Ulangi
   2. Apa fungsi dari perintah WiFi.mode(WIFI_STA) pada program tersebut?<br>
      WiFi.mode(WIFI_STA) berfungsi untuk mengatur ESP32 agar bekerja dalam mode Station (STA). Pada mode ini, ESP32 berperan sebagai perangkat/klien yang terhubung ke jaringan        WiFi yang sudah tersedia, seperti hotspot smartphone atau router
   3. Jelaskan apa yang terjadi apabila SSID atau password yang dimasukkan salah! <br>
      Apabila SSID atau password yang dimasukkan salah, ESP32 tidak dapat terhubung ke jaringan WiFi. Pada program, kondisi ini menyebabkan WiFi.status() tidak menjadi                 WL_CONNECTED, sehingga program akan terus berada di dalam perulangan while dan mencoba menunggu koneksi. Serial Monitor akan menampilkan pesan "Menghubungkan ke WiFi"            secara terus-menerus, sedangkan bagian untuk menampilkan IP Address, MAC Address, RSSI, dan menyalakan LED tidak akan dijalankan
   4. Modifikasi program agar ESP32 mencoba menghubungkan ulang (reconnect) secara otomatis apabila koneksi WiFi terputus, dan berikan penjelasan di setiap baris kode <br>
      ```cpp
      #include <WiFi.h>  // Memanggil library WiFi untuk mengatur koneksi WiFi pada ESP32

      // Menentukan nama jaringan WiFi (SSID) dan password yang digunakan
      const char* ssid = "TECNOPOVA6";
      const char* password = "hurufbesar";
      
      // Menentukan GPIO 2 sebagai pin LED indikator status koneksi
      const int ledPin = 2;
      
      void setup() {
        // Memulai komunikasi dengan Serial Monitor menggunakan baud rate 115200
        Serial.begin(115200);
      
        // Mengatur GPIO 2 sebagai output untuk mengontrol LED
        pinMode(ledPin, OUTPUT);
      
        // Mengatur LED agar dalam kondisi mati saat program pertama dijalankan
        digitalWrite(ledPin, LOW);
      
        // Mengatur ESP32 ke mode Station (STA)
        // Dalam mode ini ESP32 berperan sebagai perangkat yang terhubung
        // ke jaringan WiFi yang sudah tersedia
        WiFi.mode(WIFI_STA);
      
        // Memulai proses koneksi ESP32 ke jaringan WiFi
        // menggunakan SSID dan password yang telah ditentukan
        WiFi.begin(ssid, password);
      
        // Menampilkan pesan bahwa ESP32 sedang mencoba terhubung ke WiFi
        Serial.print("Menghubungkan ke WiFi");
      
        // Mengecek apakah ESP32 sudah berhasil terhubung ke WiFi
        // Jika belum terhubung, program akan terus menunggu
        while (WiFi.status() != WL_CONNECTED) {
          // Memberikan jeda selama 500 milidetik sebelum pengecekan berikutnya
          delay(500);
      
          // Menampilkan tanda titik sebagai indikator proses koneksi
          Serial.print(".");
        }
      
        // Bagian ini dijalankan setelah ESP32 berhasil terhubung ke WiFi
        Serial.println();
        Serial.println("WiFi berhasil terhubung");
      
        // Menampilkan IP Address yang diperoleh ESP32 dari jaringan WiFi
        Serial.print("IP Address : ");
        Serial.println(WiFi.localIP());
      
        // Menampilkan MAC Address dari ESP32
        Serial.print("MAC Address : ");
        Serial.println(WiFi.macAddress());
      
        // Menampilkan kekuatan sinyal WiFi dalam satuan dBm
        Serial.print("RSSI (dBm) : ");
        Serial.println(WiFi.RSSI());
      
        // Menyalakan LED sebagai indikator bahwa ESP32 berhasil
        // terhubung ke jaringan WiFi
        digitalWrite(ledPin, HIGH);
      }
      
      void loop() {
        // Mengecek status koneksi WiFi setiap kali loop dijalankan
        if (WiFi.status() == WL_CONNECTED) {
      
          // Jika ESP32 masih terhubung, tampilkan status pada Serial Monitor
          Serial.println("Status: Terhubung");
      
        } else {
      
          // Jika koneksi WiFi terputus, tampilkan status terputus
          Serial.println("Status: Terputus");
      
          // Mematikan LED sebagai indikator bahwa koneksi WiFi terputus
          digitalWrite(ledPin, LOW);
      
          // Mencoba menghubungkan kembali ESP32 ke jaringan WiFi
          // ketika koneksi sebelumnya terputus
          WiFi.reconnect();
      
          // Menampilkan informasi bahwa ESP32 sedang mencoba reconnect
          Serial.println("Mencoba menghubungkan kembali...");
        }
      
        // Memberikan jeda selama 5 detik sebelum mengecek status
        // koneksi WiFi kembali
        delay(5000);
      }
      ```
## 7. penjelasan detail percobaan
Pada percobaan ini, ESP32 dikonfigurasi sebagai Station (STA) untuk terhubung ke jaringan WiFi yang tersedia. ESP32 menggunakan SSID TECNOPOVA6 dan password hurufbesar. Setelah berhasil terhubung, informasi berupa IP Address, MAC Address, dan RSSI ditampilkan melalui Serial Monitor. LED pada GPIO 2 digunakan sebagai indikator keberhasilan koneksi, yaitu menyala ketika ESP32 berhasil terhubung ke WiFi.
## 8. foto percobaan
link: 
      
