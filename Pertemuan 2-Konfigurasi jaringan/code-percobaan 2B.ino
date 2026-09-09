#include <WiFi.h>  // Memanggil library WiFi untuk mengatur jaringan WiFi pada ESP32

// Menentukan nama Access Point (SSID) yang akan dibuat oleh ESP32
const char* ap_ssid = "ESP32_AccessPoint";

// Menentukan password Access Point
// Password minimal terdiri dari 8 karakter
const char* ap_password = "12345678";

void setup() {
  // Memulai komunikasi Serial Monitor dengan baud rate 115200
  Serial.begin(115200);

  // Mengatur ESP32 agar bekerja dalam mode Access Point (AP)
  // Dalam mode ini ESP32 membuat jaringan WiFi sendiri
  WiFi.mode(WIFI_AP);

  // Mengaktifkan Access Point menggunakan SSID dan password
  // yang telah ditentukan sebelumnya
  WiFi.softAP(ap_ssid, ap_password);

  // Mendapatkan alamat IP dari Access Point yang dibuat ESP32
  IPAddress apIP = WiFi.softAPIP();

  // Menampilkan informasi bahwa Access Point berhasil aktif
  Serial.println("Access Point aktif!");

  // Menampilkan nama/SSID Access Point pada Serial Monitor
  Serial.print("SSID : ");
  Serial.println(ap_ssid);

  // Menampilkan IP Address Access Point pada Serial Monitor
  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {
  // Mengambil jumlah perangkat yang sedang terhubung
  // ke Access Point ESP32
  int jumlahClient = WiFi.softAPgetStationNum();

  // Menampilkan jumlah perangkat yang terhubung
  // pada Serial Monitor
  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  // Menunggu selama 5 detik sebelum mengecek
  // jumlah perangkat yang terhubung kembali
  delay(5000);
}

