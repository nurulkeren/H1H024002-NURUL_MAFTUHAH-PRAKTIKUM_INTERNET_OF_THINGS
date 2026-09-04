# Modul 1 - Percobaan 2A

## Kendali Aktuator Relay Berdasarkan Data Sensor

### Nama

Nurul Maftuhah

### NIM

H1H024002

### Mata Kuliah

Praktikum Internet of Things (IoT)

---

## 1. Deskripsi Percobaan

Percobaan 2A membahas penggunaan data suhu dari sensor DHT22 untuk mengendalikan aktuator berupa relay. Pada percobaan ini, relay dapat disimulasikan menggunakan LED sebagai indikator.

ESP32 membaca nilai suhu dari sensor DHT22. Nilai suhu tersebut kemudian digunakan sebagai dasar untuk menentukan kondisi aktuator. Jika suhu melewati batas tertentu, aktuator akan menyala. Jika suhu turun kembali, aktuator akan dimatikan.

Pada percobaan ini program dimodifikasi menggunakan sistem **histerisis**, yaitu menggunakan dua nilai threshold yang berbeda. Aktuator akan menyala ketika suhu lebih dari **30°C**, tetapi baru akan mati ketika suhu turun di bawah **28°C**.

Dengan menggunakan dua batas tersebut, aktuator tidak mudah berubah-ubah status ketika suhu berada di sekitar 30°C.

---

## 2. Tujuan

Tujuan dari percobaan ini adalah:

1. Memahami cara menggunakan data sensor sebagai input untuk sistem kendali.
2. Menggunakan sensor DHT22 untuk membaca suhu.
3. Mengendalikan relay berdasarkan nilai suhu yang diperoleh dari sensor.
4. Menampilkan nilai suhu dan status aktuator pada Serial Monitor.
5. Memahami penggunaan percabangan `if` dan `else if`.
6. Memahami konsep histerisis pada sistem kendali.
7. Memodifikasi program agar menggunakan dua nilai threshold.

---

## 3. Alat dan Bahan

Alat dan bahan yang digunakan pada percobaan ini adalah:

* ESP32 DevKit
* Sensor DHT22
* Modul Relay 1 channel
* LED sebagai simulasi beban aktuator
* Resistor 220 Ohm
* Breadboard
* Kabel jumper
* Kabel USB
* Laptop/PC
* Arduino IDE
* DHT sensor library

Alat dan bahan tersebut sesuai dengan yang tercantum pada modul praktikum.

---

## 4. Library yang Digunakan

Library yang digunakan adalah:

```cpp
#include <DHT.h>
```

Library `DHT.h` digunakan untuk melakukan komunikasi dengan sensor DHT22.

Beberapa fungsi yang digunakan dari library tersebut adalah:

* `dht.begin()` untuk menginisialisasi sensor DHT22.
* `dht.readTemperature()` untuk membaca suhu.
* `isnan()` untuk mengecek apakah hasil pembacaan sensor valid.

Library DHT memang menyediakan fungsi pembacaan suhu, kelembaban, dan pengecekan nilai hasil pembacaan sensor.

---

## 5. Konfigurasi Pin

### Sensor DHT22

| No. | Komponen   | Pin ESP32 |
| --- | ---------- | --------- |
| 1   | VCC DHT22  | 3.3V      |
| 2   | DATA DHT22 | GPIO 4    |
| 3   | GND DHT22  | GND       |

### Relay / LED

| No. | Komponen               | Pin ESP32      |
| --- | ---------------------- | -------------- |
| 1   | IN Relay / Anoda LED   | GPIO 26        |
| 2   | VCC Relay              | 5V ESP32 (VIN) |
| 3   | GND Relay / Katoda LED | GND            |

Jika menggunakan LED sebagai pengganti relay, LED dihubungkan melalui resistor 220 Ohm.

Konfigurasi relay pada modul menggunakan GPIO 26 sebagai pin kendali.

---

## 6. Detail Percobaan

Pada percobaan ini, sensor DHT22 digunakan sebagai sumber data suhu. ESP32 membaca nilai suhu menggunakan fungsi:

```cpp
dht.readTemperature();
```

Nilai suhu yang diperoleh kemudian dibandingkan dengan nilai threshold untuk menentukan kondisi aktuator.

Pada program awal dari modul hanya digunakan satu threshold, yaitu 30°C. Jika suhu lebih dari 30°C maka relay/LED akan menyala, sedangkan jika suhu tidak melebihi 30°C maka relay/LED akan mati.

Program kemudian dimodifikasi menggunakan sistem histerisis dengan dua batas:

* **Suhu > 30°C → Aktuator ON**
* **Suhu < 28°C → Aktuator OFF**
* **Suhu 28°C sampai 30°C → Status aktuator tetap**

Modifikasi ini dilakukan agar aktuator tidak terlalu sering berpindah antara kondisi ON dan OFF ketika suhu berada di sekitar nilai threshold.

---

## 7. Kode Program Final

Berikut merupakan kode final yang digunakan setelah dilakukan modifikasi menggunakan sistem histerisis.

```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define RELAYPIN 26

DHT dht(DHTPIN, DHTTYPE);

// Batas suhu untuk menyalakan dan mematikan aktuator
const float suhuThresholdOn = 30.0;
const float suhuThresholdOff = 28.0;

// Menyimpan status aktuator
bool aktuatorStatus = false;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAYPIN, OUTPUT);

  // Memastikan aktuator mati saat awal
  digitalWrite(RELAYPIN, LOW);
}

void loop() {
  // Membaca suhu dari sensor DHT22
  float suhu = dht.readTemperature();

  // Mengecek apakah pembacaan suhu berhasil
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {

    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Menyalakan aktuator jika suhu lebih dari 30°C
    if (!aktuatorStatus && suhu > suhuThresholdOn) {
      aktuatorStatus = true;
      digitalWrite(RELAYPIN, HIGH);
    }

    // Mematikan aktuator jika suhu kurang dari 28°C
    else if (aktuatorStatus && suhu < suhuThresholdOff) {
      aktuatorStatus = false;
      digitalWrite(RELAYPIN, LOW);
    }

    // Menampilkan status aktuator
    if (aktuatorStatus) {
      Serial.println("Aktuator: ON");
    } else {
      Serial.println("Aktuator: OFF");
    }
  }

  // Jeda pembacaan sensor
  delay(2000);
}
```

---

## 8. Penjelasan Program

### 8.1 Library DHT

```cpp
#include <DHT.h>
```

Digunakan untuk memasukkan library DHT yang diperlukan agar ESP32 dapat membaca data dari sensor DHT22.

---

### 8.2 Menentukan Pin Sensor

```cpp
#define DHTPIN 4
#define DHTTYPE DHT22
```

`DHTPIN 4` menunjukkan bahwa pin DATA sensor DHT22 terhubung ke GPIO 4 ESP32.

`DHTTYPE DHT22` menunjukkan jenis sensor yang digunakan adalah DHT22.

---

### 8.3 Menentukan Pin Relay

```cpp
#define RELAYPIN 26
```

GPIO 26 digunakan sebagai pin untuk mengendalikan relay atau LED indikator.

---

### 8.4 Membuat Objek DHT

```cpp
DHT dht(DHTPIN, DHTTYPE);
```

Baris ini digunakan untuk membuat objek `dht` yang akan digunakan untuk menjalankan fungsi-fungsi sensor DHT22.

---

### 8.5 Menentukan Threshold

```cpp
const float suhuThresholdOn = 30.0;
const float suhuThresholdOff = 28.0;
```

Program menggunakan dua nilai threshold.

`suhuThresholdOn` digunakan sebagai batas untuk menyalakan aktuator, yaitu 30°C.

`suhuThresholdOff` digunakan sebagai batas untuk mematikan aktuator, yaitu 28°C.

---

### 8.6 Menyimpan Status Aktuator

```cpp
bool aktuatorStatus = false;
```

Variabel ini digunakan untuk menyimpan kondisi aktuator.

* `false` = aktuator OFF
* `true` = aktuator ON

Status ini diperlukan karena program menggunakan sistem histerisis.

---

### 8.7 Fungsi `setup()`

```cpp
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
}
```

Fungsi `setup()` dijalankan satu kali ketika ESP32 pertama kali dinyalakan.

`Serial.begin(115200)` digunakan untuk memulai komunikasi dengan Serial Monitor.

`dht.begin()` digunakan untuk menginisialisasi sensor DHT22.

`pinMode(RELAYPIN, OUTPUT)` menentukan GPIO 26 sebagai output.

`digitalWrite(RELAYPIN, LOW)` memastikan aktuator dalam kondisi mati ketika program pertama kali dijalankan.

---

### 8.8 Membaca Suhu

```cpp
float suhu = dht.readTemperature();
```

Perintah tersebut digunakan untuk mengambil data suhu dari sensor DHT22 dalam satuan Celsius.

---

### 8.9 Mengecek Pembacaan Sensor

```cpp
if (isnan(suhu)) {
  Serial.println("Gagal membaca data sensor!");
}
```

`isnan()` digunakan untuk mengecek apakah hasil pembacaan suhu merupakan nilai `NaN`.

Jika sensor gagal memberikan data suhu, program akan menampilkan pesan kesalahan.

---

### 8.10 Menyalakan Aktuator

```cpp
if (!aktuatorStatus && suhu > suhuThresholdOn) {
  aktuatorStatus = true;
  digitalWrite(RELAYPIN, HIGH);
}
```

Kondisi tersebut digunakan untuk menyalakan aktuator.

`!aktuatorStatus` berarti aktuator sedang dalam kondisi OFF.

Jika suhu lebih dari 30°C dan aktuator masih OFF, maka aktuator akan dinyalakan.

---

### 8.11 Mematikan Aktuator

```cpp
else if (aktuatorStatus && suhu < suhuThresholdOff) {
  aktuatorStatus = false;
  digitalWrite(RELAYPIN, LOW);
}
```

Kondisi tersebut digunakan untuk mematikan aktuator.

Jika aktuator sedang ON dan suhu turun di bawah 28°C, maka aktuator akan dimatikan.

---

### 8.12 Menampilkan Status Aktuator

```cpp
if (aktuatorStatus) {
  Serial.println("Aktuator: ON");
} else {
  Serial.println("Aktuator: OFF");
}
```

Bagian ini digunakan untuk menampilkan status aktuator pada Serial Monitor.

Jika `aktuatorStatus` bernilai `true`, maka status yang ditampilkan adalah `ON`.

Jika bernilai `false`, maka status yang ditampilkan adalah `OFF`.

---

## 9. Modifikasi Program

Modifikasi utama pada program adalah mengubah sistem kendali yang sebelumnya hanya menggunakan satu threshold menjadi sistem histerisis dengan dua threshold.

Program awal menggunakan satu nilai:

```cpp
const float suhuThreshold = 30.0;
```

Dengan kondisi:

```cpp
if (suhu > suhuThreshold) {
    digitalWrite(RELAYPIN, HIGH);
} else {
    digitalWrite(RELAYPIN, LOW);
}
```

Pada program modifikasi digunakan dua nilai:

```cpp
const float suhuThresholdOn = 30.0;
const float suhuThresholdOff = 28.0;
```

Kemudian status aktuator disimpan menggunakan:

```cpp
bool aktuatorStatus = false;
```

Dengan logika tersebut:

| Kondisi Suhu       | Kondisi Aktuator              |
| ------------------ | ----------------------------- |
| Suhu > 30°C        | ON                            |
| 28°C ≤ Suhu ≤ 30°C | Tetap pada kondisi sebelumnya |
| Suhu < 28°C        | OFF                           |

Modifikasi ini sesuai dengan tugas pada modul yang meminta program menggunakan dua ambang batas atau histerisis.

---

## 10. Skematik / Diagram Rangkaian

### Konfigurasi Rangkaian

```text
              ESP32 DevKit
            ┌──────────────┐
            │              │
     3.3V ──┼──────────────┼──── VCC DHT22
            │              │
   GPIO 4 ──┼──────────────┼──── DATA DHT22
            │              │
      GND ──┼──────────────┼──── GND DHT22
            │              │
   GPIO 26 ─┼──────────────┼──── IN Relay
            │              │
      VIN ──┼──────────────┼──── VCC Relay
            │              │
      GND ──┼──────────────┼──── GND Relay
            │              │
            └──────────────┘
```

Jika menggunakan LED sebagai simulasi aktuator:

```text
GPIO 26 ─── Resistor 220Ω ─── Anoda LED
                                │
                              Katoda
                                │
                               GND
```

---

## 11. Flowchart

```text
┌───────────────┐
│     MULAI     │
└───────┬───────┘
        ↓
┌──────────────────────┐
│ Inisialisasi Serial, │
│ DHT22 dan Relay      │
└──────────┬───────────┘
           ↓
┌──────────────────────┐
│ Aktuator = OFF       │
└──────────┬───────────┘
           ↓
┌──────────────────────┐
│ Baca suhu DHT22      │
└──────────┬───────────┘
           ↓
      ┌─────────────┐
      │ Data valid? │
      └──────┬──────┘
         Tidak│   │Ya
              │   ↓
              │ ┌─────────────────────┐
              │ │ Aktuator sedang OFF │
              │ │ dan suhu > 30°C?    │
              │ └──────────┬──────────┘
              │        Ya  │  Tidak
              │            ↓
              │      ┌──────────────┐
              │      │ Aktuator ON  │
              │      └──────┬───────┘
              │             │
              │             ↓
              │ ┌─────────────────────┐
              │ │ Aktuator sedang ON  │
              │ │ dan suhu < 28°C?    │
              │ └──────────┬──────────┘
              │        Ya  │  Tidak
              │            ↓
              │      ┌──────────────┐
              │      │ Aktuator OFF │
              │      └──────┬───────┘
              │             │
              └─────────────┤
                            ↓
                 ┌───────────────────┐
                 │ Tampilkan suhu dan│
                 │ status aktuator   │
                 └─────────┬─────────┘
                           ↓
                    ┌────────────┐
                    │ Delay 2 s  │
                    └─────┬──────┘
                          ↓
                       Ulangi
```

---

## 12. Jawaban Pertanyaan Praktikum

### 1. Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?

Threshold diperlukan sebagai batas untuk menentukan kapan aktuator harus menyala atau mati berdasarkan data yang diperoleh dari sensor.

Pada percobaan ini, nilai suhu digunakan sebagai dasar untuk mengendalikan relay. Ketika suhu melewati nilai threshold, ESP32 memberikan perintah kepada aktuator untuk menyala.

Dengan adanya threshold, sistem dapat mengambil keputusan secara otomatis berdasarkan kondisi lingkungan.

---

### 2. Jelaskan apa yang akan terjadi apabila nilai `suhuThreshold` diturunkan menjadi sangat rendah, misalnya 20.0°C!

Jika nilai threshold diturunkan menjadi 20°C, aktuator akan lebih mudah menyala karena suhu lingkungan biasanya dapat berada di atas nilai tersebut.

Akibatnya, kondisi untuk menyalakan aktuator akan lebih sering terpenuhi. Aktuator juga dapat menyala lebih lama atau lebih sering dibandingkan ketika threshold ditetapkan pada 30°C.

---

### 3. Jelaskan perbedaan kendali aktuator secara terus-menerus dengan kendali menggunakan histerisis!

Pada kendali menggunakan satu threshold, aktuator langsung mengikuti kondisi suhu terhadap satu nilai batas.

Contohnya:

```text
Suhu > 30°C  → ON
Suhu ≤ 30°C → OFF
```

Jika suhu berada di sekitar 30°C dan mengalami perubahan kecil, aktuator dapat sering berpindah dari ON ke OFF atau sebaliknya.

Sedangkan pada kendali menggunakan histerisis terdapat dua threshold:

```text
Suhu > 30°C → ON
Suhu < 28°C → OFF
```

Jika suhu berada di antara 28°C dan 30°C, status aktuator tidak berubah dan tetap mengikuti kondisi sebelumnya.

Dengan demikian, histerisis dapat membuat perubahan status aktuator menjadi lebih stabil.

---

### 4. Modifikasi program menggunakan dua ambang batas (histerisis)!

Program dimodifikasi dengan menggunakan dua nilai threshold:

```cpp
const float suhuThresholdOn = 30.0;
const float suhuThresholdOff = 28.0;
```

Aktuator akan menyala ketika suhu lebih dari 30°C:

```cpp
if (!aktuatorStatus && suhu > suhuThresholdOn) {
  aktuatorStatus = true;
  digitalWrite(RELAYPIN, HIGH);
}
```

Aktuator akan mati ketika suhu turun di bawah 28°C:

```cpp
else if (aktuatorStatus && suhu < suhuThresholdOff) {
  aktuatorStatus = false;
  digitalWrite(RELAYPIN, LOW);
}
```

Jika suhu berada di antara 28°C dan 30°C, status aktuator tetap seperti kondisi sebelumnya.

---

## 13. Hasil Percobaan

Setelah program berhasil di-upload ke ESP32, Serial Monitor dibuka menggunakan baud rate `115200`.

Program menampilkan nilai suhu dan status aktuator secara bersamaan.

Contoh ketika suhu masih di bawah 30°C:

```text
Suhu: 27.50 °C -> Aktuator: OFF
```

Ketika suhu naik melebihi 30°C:

```text
Suhu: 30.50 °C -> Aktuator: ON
```

Ketika suhu turun tetapi masih berada di antara 28°C dan 30°C:

```text
Suhu: 29.50 °C -> Aktuator: ON
```

Ketika suhu turun di bawah 28°C:

```text
Suhu: 27.80 °C -> Aktuator: OFF
```

**Catatan:** Nilai suhu pada contoh di atas hanya contoh. Ganti dengan hasil yang benar-benar kamu dapatkan saat praktikum.

---

## 14. Analisis Hasil

Berdasarkan percobaan yang dilakukan, ESP32 dapat membaca suhu dari sensor DHT22 dan menggunakan hasil pembacaan tersebut untuk mengendalikan aktuator.

Pada saat suhu melebihi 30°C, aktuator berubah menjadi ON. Setelah aktuator menyala, aktuator tidak langsung mati ketika suhu turun sedikit dari 30°C. Aktuator baru mati ketika suhu turun di bawah 28°C.

Hal tersebut menunjukkan bahwa sistem histerisis yang digunakan dapat menjaga kondisi aktuator agar tidak terlalu sering berubah ketika suhu berada di sekitar batas 30°C.

Hasil percobaan menunjukkan bahwa data dari sensor dapat digunakan sebagai input untuk membuat keputusan kendali secara otomatis pada sistem IoT.

---

## 15. Dokumentasi

### Foto Rangkaian
(https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link)

## 16. Kesimpulan

Pada Percobaan 2A telah dilakukan pengendalian aktuator menggunakan data suhu dari sensor DHT22.

ESP32 berhasil membaca suhu dan menggunakan nilai tersebut untuk menentukan kondisi relay atau LED. Program kemudian dimodifikasi dengan sistem histerisis menggunakan dua threshold, yaitu 30°C untuk menyalakan aktuator dan 28°C untuk mematikannya.

Penggunaan dua threshold membuat aktuator tidak langsung berubah status ketika suhu hanya mengalami perubahan kecil di sekitar 30°C. Dengan demikian, sistem kendali menjadi lebih stabil dan dapat merespons perubahan kondisi suhu secara otomatis.
