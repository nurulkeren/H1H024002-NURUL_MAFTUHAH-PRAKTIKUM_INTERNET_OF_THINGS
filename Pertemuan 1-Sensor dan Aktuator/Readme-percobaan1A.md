# Modul 1 - Percobaan 1A
## Akuisisi Data Sensor DHT22

### Nama
Nurul Maftuhah

### NIM
H1H024002

### Mata Kuliah
Praktikum Internet of Things (IoT)

---

## 1. Deskripsi Praktikum

Percobaan 1A membahas proses akuisisi data sensor menggunakan ESP32 dan sensor DHT22. Sensor DHT22 digunakan untuk membaca suhu dan kelembaban lingkungan.

Pada program awal dari modul, data suhu dan kelembaban dibaca kemudian ditampilkan pada Serial Monitor setiap 2 detik. Pada percobaan ini program dimodifikasi agar data tidak langsung ditampilkan dari satu kali pembacaan, tetapi dilakukan sebanyak 5 kali pembacaan terlebih dahulu. Setelah itu, hasil pembacaan dihitung rata-ratanya dan ditampilkan pada Serial Monitor.

Modifikasi ini dilakukan untuk mendapatkan nilai suhu dan kelembaban yang lebih mewakili hasil beberapa kali pembacaan sensor.

---

## 2. Tujuan

Tujuan dari percobaan ini adalah:

1. Memahami cara ESP32 membaca data dari sensor DHT22.
2. Membaca data suhu dan kelembaban menggunakan sensor DHT22.
3. Menampilkan hasil pembacaan sensor pada Serial Monitor.
4. Memahami penggunaan fungsi `isnan()` untuk mengecek hasil pembacaan sensor.
5. Memodifikasi program agar hasil suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan.

---

## 3. Alat dan Bahan

Alat dan bahan yang digunakan:

- ESP32 DevKit
- Sensor DHT22
- Breadboard
- Kabel jumper
- Kabel USB
- Laptop/PC
- Arduino IDE
- DHT sensor library

---

## 4. Library yang Digunakan

Library yang digunakan dalam program adalah:

```cpp
#include <DHT.h>
```
Library DHT digunakan untuk melakukan komunikasi dengan sensor DHT22 dan membaca data suhu serta kelembaban.

## 5. Konfigurasi Pin
Komponen	Pin ESP32
VCC DHT22	3.3V
DATA DHT22	GPIO 4
GND DHT22	GND

Konfigurasi pin mengikuti rangkaian pada modul, yaitu DATA DHT22 dihubungkan ke GPIO 4.

## 6. Kode Program
Berikut adalah kode final yang digunakan pada Percobaan 1A. Program telah dimodifikasi dari kode pada modul dengan menambahkan proses pembacaan sensor sebanyak 5 kali dan menghitung nilai rata-rata suhu serta kelembaban.

```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  float totalSuhu = 0;
  float totalKelembaban = 0;
  int pembacaanBerhasil = 0;

  // Melakukan pembacaan sensor sebanyak 5 kali
  for (int i = 0; i < 5; i++) {

    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();

    // Mengecek apakah pembacaan sensor berhasil
    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT22!");
    } else {
      // Menjumlahkan hasil pembacaan
      totalSuhu += suhu;
      totalKelembaban += kelembaban;

      // Menghitung jumlah pembacaan yang berhasil
      pembacaanBerhasil++;
    }

    // Jeda antar pembacaan sensor
    delay(2000);
  }

  // Mengecek apakah terdapat pembacaan yang berhasil
  if (pembacaanBerhasil > 0) {

    // Menghitung rata-rata suhu dan kelembaban
    float rataSuhu = totalSuhu / pembacaanBerhasil;
    float rataKelembaban = totalKelembaban / pembacaanBerhasil;

    Serial.println("===== HASIL RATA-RATA =====");

    Serial.print("Rata-rata Suhu: ");
    Serial.print(rataSuhu);
    Serial.println(" °C");

    Serial.print("Rata-rata Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");

    Serial.println("===========================");

  } else {
    Serial.println("Tidak ada data sensor yang berhasil dibaca.");
  }

  // Jeda sebelum melakukan pembacaan berikutnya
  delay(2000);
}
```

### Penjelasan Singkat Modifikasi

Pada program ini terdapat beberapa bagian yang dimodifikasi dari program awal pada modul.

**1. Menambahkan variabel total**

```cpp
float totalSuhu = 0;
float totalKelembaban = 0;
```
Kedua variabel digunakan untuk menyimpan jumlah seluruh hasil pembacaan suhu dan kelembaban.

**2. Menambahkan jumlah pembacaan berhasil**

```cpp
int pembacaanBerhasil = 0;
```
Variabel ini digunakan untuk menghitung berapa kali sensor berhasil memberikan data.

**3. Menambahkan perulangan sebanyak 5 kali**

```cpp
for (int i = 0; i < 5; i++) {
```
Perulangan digunakan agar sensor melakukan pembacaan sebanyak 5 kali.

**4. Menambahkan hasil pembacaan ke total**

```cpp
totalSuhu += suhu;
totalKelembaban += kelembaban;
pembacaanBerhasil++;
```
Jika pembacaan berhasil, nilai suhu dan kelembaban ditambahkan ke total. Jumlah pembacaan berhasil juga ditambah satu.

**5. Menghitung nilai rata-rata**

```cpp
float rataSuhu = totalSuhu / pembacaanBerhasil;
float rataKelembaban = totalKelembaban / pembacaanBerhasil;
```
Nilai rata-rata diperoleh dengan membagi total data dengan jumlah pembacaan yang berhasil.

Program menggunakan `pembacaanBerhasil` sebagai pembagi sehingga jika terdapat pembacaan sensor yang gagal, data yang gagal tersebut tidak ikut dihitung dalam rata-rata.

## 7. Detail Percobaan

Percobaan 1A dilakukan untuk mengetahui bagaimana ESP32 melakukan akuisisi data dari sensor DHT22. Sensor DHT22 digunakan untuk membaca dua jenis data, yaitu suhu dan kelembaban.

Pada percobaan ini, sensor DHT22 dihubungkan ke ESP32 dengan pin DATA pada GPIO 4. Program kemudian membaca data suhu dan kelembaban melalui library `DHT.h`.

Program yang digunakan merupakan modifikasi dari program pada modul. Modifikasi dilakukan dengan menambahkan proses pembacaan sebanyak 5 kali. Setiap hasil pembacaan yang berhasil akan dijumlahkan, kemudian dihitung nilai rata-ratanya sebelum ditampilkan pada Serial Monitor.

Proses pembacaan dilakukan dengan jeda 2 detik antar pembacaan. Jika sensor gagal memberikan data atau menghasilkan nilai `NaN`, program akan menampilkan pesan bahwa pembacaan sensor gagal.

Hasil akhir berupa nilai rata-rata suhu dalam satuan °C dan rata-rata kelembaban dalam satuan persen (%).

---

## 8. Skematik / Diagram Rangkaian

Rangkaian pada Percobaan 1A menggunakan ESP32 dan sensor DHT22.

### Konfigurasi Pin

| No. | Komponen   | Pin ESP32 |
| --- | ---------- | --------- |
| 1   | VCC DHT22  | 3.3V      |
| 2   | DATA DHT22 | GPIO 4    |
| 3   | GND DHT22  | GND       |

### Diagram Sederhana

```text
        ESP32 DevKit
       ┌─────────────┐
       │             │
  3.3V ├─────────────┼──── VCC
       │             │
 GPIO4 ├─────────────┼──── DATA
       │             │
   GND ├─────────────┼──── GND
       │             │
       └─────────────┘
              │
              │
         ┌───────────┐
         │   DHT22   │
         │           │
         │ VCC DATA  │
         │ GND       │
         └───────────┘
```

Pada rangkaian tersebut, pin VCC DHT22 dihubungkan ke 3.3V ESP32, pin DATA dihubungkan ke GPIO 4, dan pin GND dihubungkan ke GND ESP32.

Konfigurasi pin tersebut mengikuti konfigurasi yang diberikan pada modul praktikum.

---

# 9. Jawaban Pertanyaan Praktikum

### 1. Gambarkan diagram alur (flowchart) proses akuisisi data sensor DHT22 pada program!

Flowchart proses akuisisi data pada program adalah sebagai berikut:

```text
┌───────────────┐
│     MULAI     │
└───────┬───────┘
        ↓
┌──────────────────────┐
│ Inisialisasi Serial  │
│ dan sensor DHT22     │
└──────────┬───────────┘
           ↓
┌──────────────────────┐
│ totalSuhu = 0        │
│ totalKelembaban = 0  │
│ pembacaanBerhasil=0  │
└──────────┬───────────┘
           ↓
┌──────────────────────┐
│ Baca suhu &          │
│ kelembaban           │
└──────────┬───────────┘
           ↓
      ┌─────────────┐
      │ Data valid? │
      └──────┬──────┘
         Tidak│   │Ya
              │   ↓
              │ ┌────────────────────┐
              │ │ Tambahkan suhu dan │
              │ │ kelembaban ke total│
              │ └─────────┬──────────┘
              │           ↓
              │ ┌────────────────────┐
              │ │ pembacaanBerhasil  │
              │ │ bertambah 1        │
              │ └─────────┬──────────┘
              │           ↓
              └─────→ Delay 2 detik
                          ↓
                    ┌─────────────┐
                    │ Sudah 5 kali│
                    │ pembacaan?  │
                    └──────┬──────┘
                       Tidak│   │Ya
                            │   ↓
                            │ ┌────────────────────┐
                            │ │ Hitung rata-rata   │
                            │ │ suhu & kelembaban  │
                            │ └─────────┬──────────┘
                            │           ↓
                            │ ┌────────────────────┐
                            │ │ Tampilkan hasil    │
                            │ │ pada Serial Monitor│
                            │ └─────────┬──────────┘
                            │           ↓
                            └─────── Ulangi
```

---

### 2. Apa fungsi dari perintah `isnan()` pada program tersebut?

Fungsi `isnan()` digunakan untuk mengecek apakah nilai hasil pembacaan sensor merupakan `NaN` atau bukan angka.

Pada program digunakan:

```cpp
if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
}
```

Jika nilai suhu atau kelembaban menghasilkan `NaN`, berarti pembacaan sensor gagal. Program kemudian menampilkan pesan kesalahan pada Serial Monitor.

Jika data berhasil dibaca, program akan memasukkan nilai tersebut ke dalam perhitungan rata-rata.

---

### 3. Jelaskan mengapa diperlukan jeda (delay) minimal sekitar 2 detik antar pembacaan sensor DHT22!

Jeda sekitar 2 detik diperlukan agar sensor DHT22 memiliki waktu untuk melakukan pembacaan dan memberikan data berikutnya.

Pada program digunakan:

```cpp
delay(2000);
```

Nilai `2000` menunjukkan jeda selama 2000 milidetik atau sekitar 2 detik.

Jika pembacaan dilakukan terlalu cepat, data yang diperoleh dapat mengalami kegagalan atau tidak valid. Oleh karena itu, program memberikan jeda 2 detik setiap selesai melakukan pembacaan sensor. Modul juga menentukan bahwa data suhu dan kelembaban ditampilkan setiap 2 detik.

---

### 4. Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan!

Pada program dilakukan modifikasi dengan menambahkan proses pembacaan sensor sebanyak 5 kali.

Bagian yang digunakan adalah:

```cpp
for (int i = 0; i < 5; i++) {
```

Perulangan tersebut membuat sensor melakukan pembacaan sebanyak 5 kali.

Kemudian dibuat variabel:

```cpp
float totalSuhu = 0;
float totalKelembaban = 0;
int pembacaanBerhasil = 0;
```

`totalSuhu` digunakan untuk menyimpan jumlah seluruh nilai suhu yang berhasil dibaca.

`totalKelembaban` digunakan untuk menyimpan jumlah seluruh nilai kelembaban yang berhasil dibaca.

`pembacaanBerhasil` digunakan untuk menghitung jumlah pembacaan sensor yang berhasil.

Setiap pembacaan yang berhasil akan ditambahkan ke total:

```cpp
totalSuhu += suhu;
totalKelembaban += kelembaban;
pembacaanBerhasil++;
```

Setelah lima kali pembacaan selesai, program menghitung rata-rata:

```cpp
float rataSuhu = totalSuhu / pembacaanBerhasil;
float rataKelembaban = totalKelembaban / pembacaanBerhasil;
```

Hasil tersebut kemudian ditampilkan pada Serial Monitor.

Penggunaan `pembacaanBerhasil` sebagai pembagi membuat program hanya menghitung data yang benar-benar berhasil dibaca oleh sensor.

Modifikasi ini sesuai dengan tugas pada modul yang meminta data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan.

---

## 10. Hasil Percobaan

Setelah program di-upload ke ESP32, Serial Monitor dibuka dengan baud rate `115200`.

Program melakukan pembacaan suhu dan kelembaban sebanyak 5 kali. Setelah proses pembacaan selesai, program menampilkan hasil rata-rata suhu dan kelembaban.


## 11. Analisis Percobaan

Berdasarkan percobaan yang dilakukan, ESP32 dapat membaca data suhu dan kelembaban dari sensor DHT22.

Program tidak langsung menampilkan hasil dari satu kali pembacaan, tetapi melakukan pembacaan sebanyak 5 kali. Setiap data yang berhasil dibaca kemudian dijumlahkan dan digunakan untuk mencari nilai rata-rata.

Jika terdapat pembacaan yang gagal, program akan menampilkan pesan peringatan dan data tersebut tidak dimasukkan ke dalam perhitungan rata-rata.

Dengan menggunakan rata-rata dari beberapa pembacaan, hasil yang ditampilkan dapat memberikan gambaran yang lebih baik mengenai kondisi suhu dan kelembaban yang terbaca oleh sensor pada saat percobaan.

---

## 12. Dokumentasi

### Foto Rangkaian
https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link






