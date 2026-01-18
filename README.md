# Sensor NPK Arduino (RS485)

[![Platform](https://img.shields.io/badge/Platform-Arduino-blue.svg)]()
[![Protocol](https://img.shields.io/badge/Protocol-RS485-orange.svg)]()
[![Language](https://img.shields.io/badge/Language-C%2B%2B-brightgreen.svg)]()

EN Documentation : https://sensor-npk-arduino.superdocs.cloud/

Deskripsi:
Proyek ini memperlihatkan implementasi pembacaan nilai nutrisi tanah (N, P, K) menggunakan sensor NPK yang terhubung ke Arduino dan berkomunikasi melalui protokol RS485. Kode Arduino mengelola komunikasi RS485, pembacaan nilai sensor, kalibrasi sederhana, dan output ke Serial Monitor untuk logging atau integrasi ke perangkat lain (PLC/gateway).

Fitur utama
- Membaca nilai N, P, K dari sensor NPK
- Komunikasi jarak jauh handal via RS485 (bus multi-node)
- Konfigurasi baud rate dan alamat (address) perangkat
- Output hasil ke Serial Monitor (untuk debugging / logging)
- Arahan dasar kalibrasi dan troubleshooting

Perangkat keras yang dibutuhkan
- Arduino (Uno, Nano, Mega, dsb.)
- Sensor NPK yang mendukung RS485 (atau sensor NPK + converter RS485)
- Modul RS485 (mis. MAX485) untuk konversi TTL ↔ RS485
- Kabel jumper, power supply sesuai sensor
- (Opsional) Tampilan LCD / OLED untuk menampilkan hasil secara lokal

Skematik / Wiring (contoh umum)
- Arduino 5V -> MAX485 VCC
- GND -> MAX485 GND
- Arduino TX -> MAX485 DI
- Arduino RX <- MAX485 RO
- DE & RE (MAX485) -> Arduino digital pin (mis. D2) untuk kontrol arah
- A <-> B -> Bus RS485 (hubungkan ke A/B sensor)
- Sensor NPK (jika memakai module RS485 terpisah) -> A/B RS485 bus

Contoh koneksi pin (sesuaikan dengan kode)
- DE/RE (dir enable) = D2
- TX (Arduino) -> DI (MAX485)
- RX (Arduino) <- RO (MAX485)
- A / B dari modul MAX485 -> A / B perangkat RS485 pada sensor

Persiapan perangkat lunak
1. Install Arduino IDE (versi terbaru direkomendasikan).
2. Tambahkan library yang diperlukan (jika sketch memakai library):
   - SoftwareSerial (sudah ada di IDE)
   - ModbusMaster (jika menggunakan Modbus RTU)
   - Atau library lain yang disebutkan di sketch
3. Buka file sketch (.ino) dari repository ini dan sesuaikan:
   - Pin DE/RE
   - Baud rate (mis. 9600, 19200, 38400)
   - Alamat/ID sensor (jika sensor memakai addressing)

Konfigurasi umum (contoh)
- BAUD_RATE = 9600
- RS485_DE_RE_PIN = 2
- SENSOR_ADDRESS = 1

Cara pakai singkat
1. Sambungkan hardware seperti skematik di atas.
2. Buka sketch di Arduino IDE, sesuaikan konfigurasi.
3. Upload sketch ke board Arduino.
4. Buka Serial Monitor pada baud rate yang sama (mis. 9600).
5. Amati output nilai N, P, K secara berkala.

Contoh output Serial Monitor
```txt
[2025-10-22 17:00:01] Sensor ID: 1
N: 14.2  mg/kg
P: 7.9   mg/kg
K: 120.4 mg/kg
Status: OK
```

Kalibrasi singkat
- Pastikan sensor ditempatkan sesuai instruksi pabrikan.
- Jika hasil terlihat bias, cari parameter offset / scale di sketch:
  - offsetN, offsetP, offsetK
  - scaleN, scaleP, scaleK
- Lakukan pengukuran dengan sampel referensi dan sesuaikan nilai offset/scale hingga hasil mendekati nilai referensi.

Tips troubleshooting
- Tidak ada data di Serial Monitor:
  - Cek kabel RX/TX dan DE/RE.
  - Pastikan baud rate sama di perangkat dan Serial Monitor.
  - Periksa apakah sensor dan Arduino memiliki ground yang sama.
- Hasil naik turun / noise:
  - Periksa koneksi A/B RS485.
  - Tambahkan delay sampling lebih panjang atau averaging di kode.
- Banyak perangkat di bus RS485:
  - Pastikan setiap perangkat memiliki alamat unik.
  - Pastikan hanya satu node yang mengirim pada satu waktu (kontrol DE/RE).

Integrasi & Pengembangan lebih lanjut
- Kirim data ke gateway MQTT / database untuk monitoring jarak jauh.
- Tambahkan tampilan LCD untuk menampilkan nilai secara real-time.
- Simpan data ke SD card untuk logging.


Kontak
Jika butuh bantuan menyesuaikan README atau sketch sesuai model sensor NPK tertentu, beri tahu tipe sensor dan sketch yang dipakai — saya bantu sesuaikan wiring, konfigurasi, dan contoh penggunaan.
