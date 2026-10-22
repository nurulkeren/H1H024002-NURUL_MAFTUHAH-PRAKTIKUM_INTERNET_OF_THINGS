# Modul 5 - Percobaan 5B
# Akuisisi, Analisis, dan Visualisasi Data IoT

## 1. Detail Percobaan

Percobaan 5B membahas proses pengolahan data IoT pada sisi komputer menggunakan Python. Program dirancang untuk menerima data suhu melalui protokol MQTT, menyimpan data ke file CSV, melakukan analisis statistik, mendeteksi anomali menggunakan metode z-score, dan menampilkan data dalam bentuk grafik.

Pada percobaan ini, data suhu seharusnya diperoleh dari ESP32 yang memublikasikan data sensor melalui broker MQTT HiveMQ. Namun, karena sensor DHT22 tidak tersedia, pengujian dilakukan menggunakan data dummy atau simulasi suhu.

Data simulasi digunakan untuk menguji proses analisis statistik, perhitungan z-score, penentuan data anomali, penyimpanan CSV, serta visualisasi data. Keberhasilan pengujian MQTT secara langsung bergantung pada apakah program ESP32 atau sumber data simulasi benar-benar memublikasikan pesan ke broker.

### Spesifikasi yang Diharapkan

1. Program Python dapat menerima data suhu yang dipublikasikan melalui MQTT.
2. Data yang diterima disimpan ke file CSV beserta waktu penerimaannya.
3. Program menghitung rata-rata, simpangan baku, nilai minimum, dan maksimum.
4. Program mendeteksi anomali menggunakan metode z-score.
5. Program menampilkan grafik suhu dan menandai data yang terdeteksi sebagai anomali.
6. File `data_suhu.csv` dan `grafik_suhu.png` dihasilkan sesuai alur program.

---

# 2. Library / Dependencies

Library yang digunakan pada Percobaan 5B:

```python
import json
import time
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt
```

### Fungsi Library

| Library | Fungsi |
|---|---|
| `json` | Mengubah pesan berformat JSON menjadi objek Python. |
| `time` | Mengambil waktu penerimaan data. |
| `paho.mqtt.client` | Menjalankan komunikasi MQTT untuk menerima data dari broker. |
| `pandas` | Mengelola data, menyimpan CSV, dan menghitung statistik. |
| `matplotlib.pyplot` | Membuat grafik data suhu dan penanda anomali. |

### Instalasi Library

Jalankan perintah berikut pada terminal atau Command Prompt:

```bash
pip install paho-mqtt pandas matplotlib
```

Python 3 harus sudah terpasang pada laptop atau komputer.

---

# 3. Penjelasan Program

## A. Inisialisasi Library

```python
import json
import time
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt
```

Penjelasan:

- `import json` digunakan untuk membaca pesan dalam format JSON.
- `import time` digunakan untuk mencatat waktu penerimaan data.
- `import paho.mqtt.client as mqtt` digunakan untuk menerima pesan dari broker MQTT.
- `import pandas as pd` digunakan untuk mengelola data dalam bentuk DataFrame.
- `import matplotlib.pyplot as plt` digunakan untuk membuat grafik suhu.

## B. Konfigurasi MQTT dan Jumlah Sampel

```python
BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompokAnda/data"
JUMLAH_SAMPEL = 30

data = []
```

Penjelasan:

- `BROKER` menentukan alamat broker MQTT yang digunakan.
- `PORT = 1883` menentukan port MQTT tanpa enkripsi TLS.
- `TOPIC` menentukan topik yang akan dipantau oleh program.
- `JUMLAH_SAMPEL = 30` menentukan jumlah data yang akan dikumpulkan.
- `data = []` membuat list kosong untuk menyimpan data suhu beserta waktu penerimaannya.

**Catatan:** nilai `TOPIC` harus disesuaikan dengan topicData yang digunakan oleh program ESP32 atau sumber data simulasi.

## C. Fungsi on_connect()

```python
def on_connect(client, userdata, flags, reason_code, properties):
    print("Terhubung ke broker, kode:", reason_code)
    client.subscribe(TOPIC)
```

Penjelasan:

Fungsi `on_connect()` merupakan fungsi callback yang dijalankan ketika proses koneksi MQTT menghasilkan respons dari broker.

- `print()` menampilkan kode hasil koneksi.
- `client.subscribe(TOPIC)` mendaftarkan program agar menerima pesan dari topik yang ditentukan.

Jika koneksi berhasil, program mulai berlangganan pada topik MQTT tersebut.

## D. Fungsi on_message()

```python
def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        suhu = float(payload["suhu"])
    except (ValueError, KeyError):
        print("Data tidak valid, diabaikan")
        return

    data.append({
        "waktu": time.strftime("%Y-%m-%d %H:%M:%S"),
        "suhu": suhu
    })

    print(f"[{len(data)}/{JUMLAH_SAMPEL}] suhu = {suhu}")

    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()
```

Penjelasan:

1. `def on_message()` membuat fungsi yang dijalankan ketika pesan baru diterima dari topik yang di-subscribe.
2. `try` mencoba memproses pesan yang masuk.
3. `msg.payload.decode()` mengubah payload dari byte menjadi teks.
4. `json.loads()` mengubah teks JSON menjadi objek Python.
5. `float(payload["suhu"])` mengambil nilai suhu dan mengubahnya menjadi tipe numerik.
6. `except (ValueError, KeyError)` menangani kesalahan konversi nilai atau key suhu yang tidak tersedia.
7. `return` menghentikan pemrosesan pesan yang tidak valid.
8. `data.append()` menyimpan waktu penerimaan dan nilai suhu ke dalam list.
9. `print()` menampilkan jumlah data yang diterima dan nilai suhu.
10. `if len(data) >= JUMLAH_SAMPEL` memeriksa apakah jumlah sampel telah mencapai target.
11. `client.disconnect()` memutus koneksi MQTT setelah jumlah sampel terpenuhi.

Pesan yang diterima harus memiliki format JSON yang berisi key `suhu`, misalnya:

```json
{
  "suhu": 28.5
}
```

## E. Tahap 1: Akuisisi Data dari MQTT

```python
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_forever()
```

Penjelasan:

- `mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)` membuat objek MQTT client menggunakan Callback API versi 2.
- `client.on_connect = on_connect` menetapkan fungsi callback koneksi.
- `client.on_message = on_message` menetapkan fungsi callback penerimaan pesan.
- `client.connect(BROKER, PORT)` mencoba menghubungkan program ke broker.
- `client.loop_forever()` menjalankan proses jaringan MQTT sampai koneksi dihentikan.

Program akan menunggu pesan masuk dari topik yang sudah didaftarkan. Pengumpulan data berakhir ketika callback menerima 30 data dan menjalankan `client.disconnect()`.

## F. Tahap 2: Menyimpan Data ke CSV

```python
df = pd.DataFrame(data)
df.to_csv("data_suhu.csv", index=False)
print("Data tersimpan di data_suhu.csv")
```

Penjelasan:

- `pd.DataFrame(data)` mengubah list data menjadi tabel DataFrame.
- `df.to_csv("data_suhu.csv", index=False)` menyimpan data ke file CSV tanpa menambahkan indeks DataFrame sebagai kolom.
- `print()` menampilkan informasi bahwa proses penyimpanan selesai.

File CSV memiliki dua kolom utama, yaitu `waktu` dan `suhu`.

Contoh isi file:

| waktu | suhu |
|---|---:|
| 2026-10-22 10:00:00 | 28.5 |
| 2026-10-22 10:00:05 | 28.7 |
| 2026-10-22 10:00:10 | 29.1 |

Angka di atas hanya contoh ilustrasi, bukan hasil pengukuran yang sebenarnya.

## G. Tahap 3: Analisis Statistik dan Deteksi Anomali

```python
rata2 = df["suhu"].mean()
std = df["suhu"].std()

if std == 0:
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2
```

Penjelasan:

- `df["suhu"].mean()` menghitung rata-rata suhu.
- `df["suhu"].std()` menghitung simpangan baku sampel.
- `if std == 0` memeriksa apakah simpangan baku bernilai nol.
- `df["zscore"] = 0.0` memberikan nilai z-score nol ketika semua data memiliki nilai yang sama.
- `else` menjalankan perhitungan z-score ketika simpangan baku tidak nol.
- `(df["suhu"] - rata2) / std` menghitung jarak setiap data dari rata-rata dalam satuan simpangan baku.
- `df["zscore"].abs() > 2` menandai data dengan nilai absolut z-score lebih besar dari 2 sebagai anomali.

Rumus z-score:

z = (x - rata-rata) / simpangan baku

Nilai z-score menunjukkan seberapa jauh suatu data dari rata-rata. Dalam praktikum ini, data dengan nilai absolut z-score lebih dari 2 dianggap sebagai anomali.

Selanjutnya, program menampilkan hasil analisis:

```python
print(f"Rata-rata : {rata2:.2f}")
print(f"Std deviasi: {std:.2f}")
print(f"Minimum   : {df['suhu'].min():.2f}")
print(f"Maksimum : {df['suhu'].max():.2f}")
print(f"Jumlah anomali: {df['anomali'].sum()}")
```

Penjelasan:

- `rata2` menampilkan rata-rata suhu dengan dua angka desimal.
- `std` menampilkan simpangan baku.
- `df["suhu"].min()` menampilkan suhu minimum.
- `df["suhu"].max()` menampilkan suhu maksimum.
- `df["anomali"].sum()` menghitung jumlah data yang ditandai sebagai anomali.

## H. Tahap 4: Visualisasi Data

```python
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"], marker="o", label="Suhu")

anom = df[df["anomali"]]

plt.scatter(
    anom.index,
    anom["suhu"],
    color="red",
    s=100,
    label="Anomali",
    zorder=3
)

plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata")
plt.xlabel("Urutan data")
plt.ylabel("Suhu (°C)")
plt.title("Data Suhu dan Deteksi Anomali")
plt.legend()
plt.tight_layout()
plt.savefig("grafik_suhu.png")
plt.show()
```

Penjelasan:

- `plt.figure(figsize=(10, 4))` membuat area grafik berukuran 10 × 4 inci.
- `plt.plot()` membuat grafik garis suhu dengan penanda berbentuk lingkaran.
- `df[df["anomali"]]` memilih baris yang ditandai sebagai anomali.
- `plt.scatter()` menampilkan titik anomali dengan warna merah.
- `plt.axhline()` menambahkan garis horizontal pada nilai rata-rata suhu.
- `plt.xlabel()` memberi label sumbu horizontal.
- `plt.ylabel()` memberi label sumbu vertikal.
- `plt.title()` memberikan judul grafik.
- `plt.legend()` menampilkan keterangan grafik.
- `plt.tight_layout()` merapikan tata letak grafik.
- `plt.savefig("grafik_suhu.png")` menyimpan grafik ke file PNG.
- `plt.show()` menampilkan grafik pada layar.

---

# 4. Penjelasan Tiap Fungsi

| No. | Fungsi / Bagian Kode | Penjelasan |
|---|---|---|
| 1 | `import json` | Memproses pesan dalam format JSON. |
| 2 | `import time` | Mengambil waktu penerimaan data. |
| 3 | `paho.mqtt.client` | Menjalankan komunikasi MQTT. |
| 4 | `pandas` | Mengelola data tabel dan menghitung statistik. |
| 5 | `matplotlib.pyplot` | Membuat dan menyimpan grafik. |
| 6 | `BROKER` | Menentukan alamat broker MQTT. |
| 7 | `PORT` | Menentukan port komunikasi MQTT. |
| 8 | `TOPIC` | Menentukan topik yang di-subscribe. |
| 9 | `JUMLAH_SAMPEL` | Menentukan target jumlah data yang dikumpulkan. |
| 10 | `data = []` | Menyimpan data yang diterima. |
| 11 | `on_connect()` | Menangani respons koneksi dan melakukan subscribe. |
| 12 | `on_message()` | Memproses pesan yang diterima dari MQTT. |
| 13 | `json.loads()` | Mengubah teks JSON menjadi objek Python. |
| 14 | `float()` | Mengubah nilai suhu menjadi tipe numerik. |
| 15 | `data.append()` | Menambahkan data suhu dan waktu ke list. |
| 16 | `client.subscribe()` | Mendaftarkan topik untuk menerima pesan. |
| 17 | `client.loop_forever()` | Menjalankan loop komunikasi MQTT. |
| 18 | `pd.DataFrame()` | Mengubah data menjadi tabel. |
| 19 | `df.to_csv()` | Menyimpan tabel ke file CSV. |
| 20 | `mean()` | Menghitung rata-rata suhu. |
| 21 | `std()` | Menghitung simpangan baku sampel. |
| 22 | `zscore` | Menyimpan nilai z-score setiap data. |
| 23 | `anomali` | Menandai data dengan nilai absolut z-score lebih dari 2. |
| 24 | `min()` | Menghitung suhu minimum. |
| 25 | `max()` | Menghitung suhu maksimum. |
| 26 | `plt.plot()` | Membuat grafik garis suhu. |
| 27 | `plt.scatter()` | Memberi penanda pada data anomali. |
| 28 | `plt.axhline()` | Menampilkan garis rata-rata. |
| 29 | `plt.savefig()` | Menyimpan grafik sebagai file PNG. |
| 30 | `plt.show()` | Menampilkan grafik pada layar. |

---

# 5. Penjelasan Percabangan dan Conditional

| No. | Percabangan / Conditional | Penjelasan |
|---|---|---|
| 1 | `try` | Mencoba memproses pesan JSON dan mengambil nilai suhu. |
| 2 | `except (ValueError, KeyError)` | Menangani nilai suhu yang tidak dapat dikonversi atau key `suhu` yang tidak tersedia. |
| 3 | `return` | Menghentikan pemrosesan pesan yang tidak valid. |
| 4 | `if len(data) >= JUMLAH_SAMPEL` | Memeriksa apakah jumlah sampel sudah mencapai target. |
| 5 | `if std == 0` | Memeriksa apakah simpangan baku bernilai nol. |
| 6 | `else` pada perhitungan z-score | Menghitung z-score jika simpangan baku tidak nol. |
| 7 | `df["zscore"].abs() > 2` | Menentukan apakah data ditandai sebagai anomali. |

---

# 6. Jawaban Pertanyaan Praktikum

## 1. Jelaskan fungsi blok try-except pada fungsi on_message() dan apa yang terjadi apabila blok tersebut dihilangkan!

Blok `try-except` digunakan untuk menangani kesalahan ketika program membaca pesan JSON dan mengubah nilai suhu menjadi tipe numerik.

Bagian `try` mencoba menjalankan proses penguraian JSON dan mengambil nilai pada key `suhu`. Jika nilai tidak dapat dikonversi atau key tersebut tidak tersedia, blok `except` akan menampilkan pesan bahwa data tidak valid, kemudian `return` menghentikan pemrosesan pesan tersebut.

Jika blok `try-except` dihilangkan, kesalahan seperti `ValueError` atau `KeyError` dapat menghentikan callback dan mengganggu proses penerimaan data. Dengan adanya penanganan kesalahan, program lebih mampu menghadapi pesan yang tidak sesuai format.

Catatan: blok `except` pada kode asli hanya menangani `ValueError` dan `KeyError`. Kesalahan JSON seperti `json.JSONDecodeError` tidak secara eksplisit dicantumkan, sehingga penanganannya dapat diperluas jika diperlukan.

## 2. Jelaskan arti nilai z-score dan mengapa batas |z| > 2 digunakan sebagai penentu anomali!

Z-score menunjukkan jarak suatu data dari rata-rata dalam satuan simpangan baku. Nilai z-score positif berarti data berada di atas rata-rata, sedangkan nilai negatif berarti data berada di bawah rata-rata.

Dalam praktikum ini, data dengan nilai absolut z-score lebih dari 2 ditandai sebagai anomali karena data tersebut berjarak lebih dari dua simpangan baku dari rata-rata.

Batas ini merupakan aturan sederhana untuk mengidentifikasi nilai yang relatif jauh dari pusat distribusi data. Namun, penanda anomali tidak selalu berarti data salah karena perubahan kondisi lingkungan yang nyata juga dapat menghasilkan nilai ekstrem.

## 3. Apa kelemahan metode z-score apabila data memiliki tren naik atau turun yang tajam?

Metode z-score menggunakan rata-rata dan simpangan baku seluruh data sebagai dasar perhitungan. Jika data memiliki tren naik atau turun yang tajam, nilai pada awal dan akhir pengamatan dapat berbeda secara alami.

Akibatnya, data yang sebenarnya merupakan bagian dari tren dapat ditandai sebagai anomali. Sebaliknya, anomali tertentu juga dapat tidak terdeteksi jika rata-rata dan simpangan baku ikut terpengaruh oleh nilai ekstrem.

Oleh karena itu, metode z-score perlu dipertimbangkan bersama karakteristik data. Untuk data yang memiliki tren, analisis berbasis jendela waktu atau metode deteksi anomali lainnya dapat lebih sesuai.

## 4. Modifikasi program agar menggunakan moving average dengan jendela 5 data sebelum menghitung z-score, kemudian bandingkan jumlah anomali!

Untuk menerapkan moving average lima data pada data CSV, kode berikut dapat ditambahkan setelah file CSV dibaca dan sebelum perhitungan z-score dilakukan.

```python
# Menghitung moving average dengan jendela 5 data
df["suhu_ma"] = df["suhu"].rolling(
    window=5,
    min_periods=1
).mean()

# Menghitung rata-rata dan simpangan baku hasil filter
rata2_ma = df["suhu_ma"].mean()
std_ma = df["suhu_ma"].std()

# Menghitung z-score data hasil moving average
if pd.isna(std_ma) or std_ma == 0:
    df["zscore_ma"] = 0.0
else:
    df["zscore_ma"] = (
        df["suhu_ma"] - rata2_ma
    ) / std_ma

# Menentukan anomali pada data hasil filter
df["anomali_ma"] = df["zscore_ma"].abs() > 2

# Membandingkan jumlah anomali sebelum dan sesudah filter
print("Anomali sebelum moving average:",
      df["anomali"].sum())

print("Anomali setelah moving average:",
      df["anomali_ma"].sum())
```

Penjelasan:

1. `rolling(window=5, min_periods=1).mean()` menghitung rata-rata bergerak dengan jendela lima data. Pada awal data, rata-rata dihitung dari sampel yang tersedia.
2. `rata2_ma` menghitung rata-rata kolom hasil moving average.
3. `std_ma` menghitung simpangan baku hasil filter.
4. Kondisi `if pd.isna(std_ma) or std_ma == 0` mencegah perhitungan z-score dengan simpangan baku yang tidak tersedia atau nol.
5. `zscore_ma` menyimpan z-score dari data yang sudah difilter.
6. `anomali_ma` menandai data hasil filter yang memiliki nilai absolut z-score lebih dari 2.
7. Dua perintah `print()` membandingkan jumlah anomali sebelum dan sesudah moving average.

Jumlah anomali setelah filter tidak dapat dipastikan sebelum program dijalankan karena bergantung pada nilai suhu yang digunakan. Moving average cenderung menghaluskan fluktuasi, tetapi tidak menjamin jumlah anomali selalu berkurang.

---

# 7. Hasil Percobaan

Percobaan 5B bertujuan menguji pengolahan data suhu pada komputer menggunakan Python. Proses yang dirancang meliputi penerimaan data melalui MQTT, penyimpanan ke CSV, analisis statistik, deteksi anomali dengan z-score, dan visualisasi grafik.

Karena sensor DHT22 tidak tersedia, data dummy digunakan untuk menguji bagian pengolahan dan analisis data. Pada pengujian yang menggunakan data simulasi secara langsung, program dapat diuji untuk menghitung rata-rata, simpangan baku, nilai minimum dan maksimum, serta menandai anomali dan membuat grafik.

Pengujian penerimaan data melalui broker MQTT perlu dibedakan dari pengujian analisis data secara lokal. Jika data dummy dimasukkan langsung ke program tanpa dikirim melalui broker, hal tersebut hanya membuktikan proses analisis dan visualisasi, bukan keberhasilan komunikasi MQTT secara langsung.

Berkas keluaran yang dirancang pada percobaan ini adalah:

1. `data_suhu.csv` untuk menyimpan data suhu dan waktu penerimaan.
2. `grafik_suhu.png` untuk menyimpan grafik suhu beserta penanda anomali.

---

# 8. Analisis

Berdasarkan rancangan program, penggunaan Python memungkinkan data suhu dianalisis dengan lebih fleksibel dibandingkan pengolahan sederhana langsung pada ESP32. Library pandas memudahkan perhitungan statistik dan pengelolaan data, sedangkan matplotlib memudahkan penyajian data dalam bentuk grafik.

Metode z-score digunakan untuk mengidentifikasi data yang relatif jauh dari rata-rata. Penanda anomali pada grafik membantu pengamatan terhadap nilai suhu yang menyimpang. Meskipun demikian, hasil deteksi bergantung pada karakteristik data yang digunakan dan ambang batas yang ditentukan.

Penggunaan data dummy memungkinkan pengujian logika statistik dan visualisasi meskipun sensor tidak tersedia. Akan tetapi, hasil simulasi tidak dapat digunakan sebagai bukti bahwa komunikasi MQTT dengan ESP32 maupun pembacaan suhu DHT22 telah berhasil dilakukan secara langsung.

---

# 9. Kesimpulan

Percobaan 5B dirancang untuk menerapkan akuisisi, analisis, dan visualisasi data IoT menggunakan Python. Program mencakup penerimaan data MQTT, pencatatan waktu, penyimpanan CSV, perhitungan statistik, deteksi anomali menggunakan z-score, serta pembuatan grafik.

Data dummy dapat digunakan untuk menguji proses pengolahan dan analisis tanpa sensor DHT22. Keberhasilan setiap bagian perlu dinilai berdasarkan pengujian yang benar-benar dilakukan, khususnya untuk membedakan pengujian data simulasi lokal dengan pengujian komunikasi MQTT secara langsung.

Dengan demikian, percobaan ini memberikan pemahaman mengenai tahapan pengolahan data IoT pada sisi komputer dan cara menggunakan analisis statistik serta visualisasi untuk membantu memahami data sensor.

---

# 10. Foto Percobaan
link: https://drive.google.com/drive/folders/1bQNmdO6_lQoJMbp50PNAGneGszcxOYip?usp=drive_link

