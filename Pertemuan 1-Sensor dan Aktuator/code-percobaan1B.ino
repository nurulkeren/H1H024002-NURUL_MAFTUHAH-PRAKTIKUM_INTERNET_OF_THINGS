```cpp
#include <DHT.h>

// Pin dan tipe sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22

// Pin untuk relay
#define RELAYPIN 26

// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

// Threshold suhu untuk mengatur kondisi relay
const float suhuThresholdOn = 30.0;
const float suhuThresholdOff = 28.0;

// Menyimpan status relay
bool relayStatus = false;

void setup() {
  // Memulai komunikasi Serial dengan baud rate 115200
  Serial.begin(115200);

  // Inisialisasi sensor DHT22
  dht.begin();

  // Mengatur pin relay sebagai output
  pinMode(RELAYPIN, OUTPUT);

  // Memastikan relay dalam kondisi mati saat awal
  digitalWrite(RELAYPIN, LOW);

  Serial.println("Memulai sistem kendali relay berdasarkan suhu...");
}

void loop() {
  // Membaca suhu dari sensor DHT22
  float suhu = dht.readTemperature();

  // Mengecek apakah pembacaan sensor berhasil
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
  } else {

    // Menampilkan nilai suhu
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C");

    // Menyalakan relay jika suhu lebih dari 30°C
    if (!relayStatus && suhu > suhuThresholdOn) {
      relayStatus = true;
      digitalWrite(RELAYPIN, HIGH);

      Serial.println(" -> Relay ON");
    }

    // Mematikan relay jika suhu kurang dari 28°C
    else if (relayStatus && suhu < suhuThresholdOff) {
      relayStatus = false;
      digitalWrite(RELAYPIN, LOW);

      Serial.println(" -> Relay OFF");
    }

    // Jika suhu berada di antara 28°C dan 30°C,
    // relay mempertahankan kondisi sebelumnya
    else {
      if (relayStatus) {
        Serial.println(" -> Relay tetap ON");
      } else {
        Serial.println(" -> Relay tetap OFF");
      }
    }
  }

  // Jeda sebelum melakukan pembacaan berikutnya
  delay(2000);
}
```

