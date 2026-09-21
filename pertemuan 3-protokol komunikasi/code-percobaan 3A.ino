#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Menentukan nama jaringan WiFi dan password
const char* ssid = "TECNOPOVA6";
const char* password = "hurufbesar";

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
