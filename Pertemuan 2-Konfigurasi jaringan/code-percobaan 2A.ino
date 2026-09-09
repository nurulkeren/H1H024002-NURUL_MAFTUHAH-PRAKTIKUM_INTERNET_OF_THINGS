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

