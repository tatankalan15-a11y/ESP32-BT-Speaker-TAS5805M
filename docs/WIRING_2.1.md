# Wiring Diagram: ESP32 Bluetooth Speaker 2.1 dengan 2x TAS5805M

## Konfigurasi Sistem

```
Smartphone/Laptop
      ↓ Bluetooth A2DP
      ↓
  ESP32 DevKit
    ↙    ↓    ↘
   I2C  I2S  I2C_Sub
   Main      Sub
    ↓        ↓
TAS5805M  TAS5805M
  (Main)    (Sub)
    ↙        ↘
  L/R        Mono
   ↓         Sub
Speaker   Subwoofer
```

## Pinout Detail

### ESP32 DevKit

```
┌─────────────────────────────────────────────────────┐
│             ESP32 DevKit                            │
├─────────────────────────────────────────────────────┤
│                                                     │
│  I2C (Main TAS5805M - Stereo L/R)                  │
│  ├─ GPIO 21 (SDA) ────→ TAS5805M_Main SDA          │
│  ├─ GPIO 22 (SCL) ────→ TAS5805M_Main SCL          │
│  └─ GND ──────────────→ TAS5805M_Main GND          │
│                                                     │
│  I2C (Secondary TAS5805M - Mono Subwoofer)         │
│  ├─ GPIO 4  (SDA) ────→ TAS5805M_Sub SDA           │
│  ├─ GPIO 5  (SCL) ────→ TAS5805M_Sub SCL           │
│  └─ GND ──────────────→ TAS5805M_Sub GND           │
│                                                     │
│  I2S (Audio Data Stream - both DACs)               │
│  ├─ GPIO 25 (LRCLK/WS) ──→ TAS5805M_Main WS       │
│  ├─ GPIO 25 (LRCLK/WS) ──→ TAS5805M_Sub WS        │
│  ├─ GPIO 26 (BCLK/SCK)  ──→ TAS5805M_Main SCK     │
│  ├─ GPIO 26 (BCLK/SCK)  ──→ TAS5805M_Sub SCK      │
│  ├─ GPIO 14 (DIN/SD)    ──→ TAS5805M_Main DIN     │
│  ├─ GPIO 14 (DIN/SD)    ──→ TAS5805M_Sub DIN      │
│  └─ GND ─────────────────→ Common GND              │
│                                                     │
│  Power                                              │
│  ├─ 5V USB ──→ ESP32 VCC                           │
│  └─ GND ─────→ Common GND                          │
│                                                     │
└─────────────────────────────────────────────────────┘
```

## TAS5805M_Main (Stereo L/R)

```
ESP32                           TAS5805M_Main
  SDA(21) ───────────────────── SDA
  SCL(22) ───────────────────── SCL
  LRCLK(25) ──────────────────── WS
  BCLK(26) ───────────────────── SCK
  DIN(14) ────────────────────── DIN
  GND ─────────────────────────── GND
                                  |
                              15V+ Power (external recommended)
                              GND (common)
                                  |
                            ┌─────┴─────┐
                          OUT_L       OUT_R
                            ↓           ↓
                        Speaker L   Speaker R
```

## TAS5805M_Sub (Mono Subwoofer)

```
ESP32                           TAS5805M_Sub
  SDA(4) ────────────────────── SDA
  SCL(5) ────────────────────── SCL
  LRCLK(25) ──────────────────── WS
  BCLK(26) ───────────────────── SCK
  DIN(14) ────────────────────── DIN
  GND ─────────────────────────── GND
                                  |
                              15V+ Power (external recommended)
                              GND (common)
                                  |
                            ┌─────┴─────┐
                          OUT_L       OUT_R
                            ↓           ↓
                        Subwoofer L  Subwoofer R
                    (dapat dijumper jadi mono)
```

## Power Supply

### Opsi 1: USB Power (Sederhana)
- ESP32 powered by 5V USB
- TAS5805M_Main & TAS5805M_Sub powered by 15V USB adapter (or 5V, tapi gain lebih rendah)
- GND harus common

### Opsi 2: External Power Supply (Recommended)
- ESP32 powered by 5V USB
- TAS5805M_Main & TAS5805M_Sub powered by 15V external supply
- GND harus common

## Wiring Checklist

- [ ] ESP32 SDA(21) → TAS5805M_Main SDA
- [ ] ESP32 SCL(22) → TAS5805M_Main SCL
- [ ] ESP32 SDA(4)  → TAS5805M_Sub SDA
- [ ] ESP32 SCL(5)  → TAS5805M_Sub SCL
- [ ] ESP32 LRCLK(25) → TAS5805M_Main WS
- [ ] ESP32 LRCLK(25) → TAS5805M_Sub WS
- [ ] ESP32 BCLK(26) → TAS5805M_Main SCK
- [ ] ESP32 BCLK(26) → TAS5805M_Sub SCK
- [ ] ESP32 DIN(14) → TAS5805M_Main DIN
- [ ] ESP32 DIN(14) → TAS5805M_Sub DIN
- [ ] ESP32 GND → TAS5805M_Main GND
- [ ] ESP32 GND → TAS5805M_Sub GND
- [ ] TAS5805M_Main OUT_L → Speaker L
- [ ] TAS5805M_Main OUT_R → Speaker R
- [ ] TAS5805M_Sub OUT_L → Subwoofer L
- [ ] TAS5805M_Sub OUT_R → Subwoofer R (atau Common if mono)
- [ ] 15V+ → TAS5805M_Main VCC
- [ ] 15V+ → TAS5805M_Sub VCC
- [ ] GND → Common Ground (all)

## Audio Flow

```
Bluetooth Audio Stream (Stereo L/R)
            ↓
        ESP32 I2S
      (25 LRCLK, 26 BCLK, 14 DIN)
            ↓
    ┌───────┴────────┐
    ↓                ↓
TAS5805M_Main    TAS5805M_Sub
(Stereo L/R)     (Mono Sub)
    ↓                ↓
Speaker L/R      Subwoofer
```

**Catatan**: I2S data mengalir ke kedua TAS5805M secara paralel. Masing-masing mengolah data stereo, tapi yang penting adalah:
- TAS5805M_Main output OUT_L dan OUT_R untuk speaker utama
- TAS5805M_Sub bisa di-configure untuk output mono subwoofer atau stereo sub

## Konfigurasi Alternatif

Jika Anda ingin menggunakan pin I2S yang berbeda, edit `platformio.ini`:

```ini
-D PIN_I2S_FS=25    # Ganti dengan pin LRCLK yang lain
-D PIN_I2S_SCK=26   # Ganti dengan pin BCLK yang lain
-D PIN_I2S_SD=14    # Ganti dengan pin DIN yang lain
```

Jika Anda ingin menggunakan pin I2C yang berbeda untuk subwoofer:

```ini
-D PIN_I2C_SDA_SUB=4   # Ganti dengan SDA pin lain
-D PIN_I2C_SCL_SUB=5   # Ganti dengan SCL pin lain
```

## Troubleshooting

| Issue | Solusi |
|-------|--------|
| Tidak ada audio | Cek I2S wiring (25, 26, 14); Cek power TAS5805M |
| Stereo kiri tidak keluar | Cek TAS5805M_Main OUT_L wiring |
| Stereo kanan tidak keluar | Cek TAS5805M_Main OUT_R wiring |
| Subwoofer tidak keluar | Cek I2C SDA(4), SCL(5); Cek power TAS5805M_Sub |
| I2C error pada TAS5805M_Sub | Cek SDA(4) dan SCL(5) tidak tertukar; Pastikan pull-up resistor |
| Gain terlalu rendah | Gunakan external 15V+ power supply untuk gain maksimal |
| Clipping audio | Turunkan volume atau gunakan TAS5805M_MIN_GAIN |

## Testing

1. **Build dan Upload**:
   ```bash
   platformio run -e esp32-bt-speaker-2ch-1
   platformio run -t upload -e esp32-bt-speaker-2ch-1
   ```

2. **Monitor Serial**:
   ```bash
   platformio device monitor -b 115200
   ```

3. **Pair Bluetooth**:
   - Cari "ESP32_BT_Speaker_2.1"
   - Connect
   - Play music

4. **Test Output**:
   - Periksa stereo L/R keluar dari speaker
   - Periksa subwoofer bass keluar dari subwoofer
