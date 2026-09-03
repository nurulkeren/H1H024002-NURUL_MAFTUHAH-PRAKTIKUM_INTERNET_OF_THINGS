#include <DHT.h>

// Pin dan tipe sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22

// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

// Jumlah pembacaan yang digunakan untuk mencari rata-rata
const int jumlahPembacaan = 5;

void setup() {
  // Memulai komunikasi Serial dengan baud rate 115200
  Serial.begin(115200);

  // Inisialisasi sensor DHT22
  dht.begin();

  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  // Variabel untuk menyimpan total suhu dan kelembaban
  float totalSuhu = 0;
  float totalKelembaban = 0;

  // Menyimpan status apakah pembacaan sensor berhasil
  bool pembacaanBerhasil = true;

  // Melakukan pembacaan sensor sebanyak 5 kali
  for (int i = 0; i < jumlahPembacaan; i++) {

    // Membaca suhu dan kelembaban dari sensor
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();

    // Mengecek apakah data yang diperoleh valid
    if (isnan(suhu) || isnan(kelembaban)) {
      pembacaanBerhasil = false;
      break;
    }

    // Menambahkan hasil pembacaan ke total
    totalSuhu += suhu;
    totalKelembaban += kelembaban;

    // Memberikan jeda antar pembacaan
    delay(2000);
  }

  // Jika semua pembacaan berhasil
  if (pembacaanBerhasil) {

    // Menghitung nilai rata-rata suhu
    float rataSuhu = totalSuhu / jumlahPembacaan;

    // Menghitung nilai rata-rata kelembaban
    float rataKelembaban = totalKelembaban / jumlahPembacaan;

    // Menampilkan hasil ke Serial Monitor
    Serial.print("Rata-rata Suhu: ");
    Serial.print(rataSuhu);
    Serial.print(" °C, Rata-rata Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");

  } else {

    // Menampilkan pesan jika pembacaan sensor gagal
    Serial.println("Gagal membaca data dari sensor DHT22!");
  }

  // Jeda sebelum melakukan proses pembacaan berikutnya
  delay(2000);
}
