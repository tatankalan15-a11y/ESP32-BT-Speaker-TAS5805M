# ESP32 Bluetooth Speaker with TAS5805M

**Wireless Audio Receiver untuk Speaker 2.0 dan 2.1**

Firmware ESP32 yang berfungsi sebagai Bluetooth audio receiver (A2DP sink) dengan amplifier TAS5805M Class-D.

## 🎯 Fitur Utama

- ✅ Penerima Audio Bluetooth A2DP (wireless speaker)
- ✅ Output Audio I2S ke TAS5805M
- ✅ Dukungan 2.0 (1x TAS5805M Stereo)
- ✅ Dukungan 2.1 (2x TAS5805M: Stereo + Mono Subwoofer)
- ✅ Kontrol Volume otomatis via TAS5805M
- ✅ Status Koneksi Real-time

## 📋 Quick Comparison

| Fitur | 2.0 | 2.1 |
|-------|-----|-----|
| DAC | 1x TAS5805M | 2x TAS5805M |
| Output | Stereo L/R | Stereo L/R + Mono Sub |
| I2C | 1 (SDA=21, SCL=22) | 2 (Main + Sub) |
| Speaker | Speaker L/R | Speaker L/R + Subwoofer |
| Kompleksitas | Rendah | Sedang |
| Power | 5V USB atau 15V+ | 15V+ (recommended) |

## 🔌 Hardware Requirements

### 2.0 System
- ESP32 DevKit
- 1x TAS5805M Module
- 2x Speaker (3-10W stereo)
- Power: 5V USB atau 15V+ external

### 2.1 System
- ESP32 DevKit
- 2x TAS5805M Module
- 2x Speaker (3-10W stereo)
- 1x Subwoofer amplifier (mono or stereo)
- Power: 15V+ external (recommended)

## 📍 Pin Configuration

### 2.0 Configuration

```
ESP32 → TAS5805M
  SDA(21) → SDA
  SCL(22) → SCL
  LRCLK(25) → WS
  BCLK(26) → SCK
  DIN(14) → DIN
  GND → GND

TAS5805M → Speaker
  OUT_L → Speaker Left
  OUT_R → Speaker Right
```

### 2.1 Configuration

```
ESP32 → TAS5805M_Main (I2C: SDA=21, SCL=22)
ESP32 → TAS5805M_Sub (I2C: SDA=4, SCL=5)

Both DACs share I2S pins:
  LRCLK(25) → WS
  BCLK(26) → SCK
  DIN(14) → DIN

TAS5805M_Main → Speaker
  OUT_L → Speaker Left
  OUT_R → Speaker Right

TAS5805M_Sub → Subwoofer
  OUT_L → Sub Left (or mono)
  OUT_R → Sub Right (or mono)
```

Lihat `docs/WIRING_2.1.md` untuk detail lengkap.

## 🚀 Quick Start

### 1. Clone Repository
```bash
git clone https://github.com/tatankalan15-a11y/ESP32-BT-Speaker-TAS5805M.git
cd ESP32-BT-Speaker-TAS5805M
```

### 2. Install PlatformIO
```bash
pip install platformio
```

### 3. Build 2.0 (Single TAS5805M)
```bash
# Build
platformio run -e esp32-bt-speaker-2ch

# Upload
platformio run -t upload -e esp32-bt-speaker-2ch

# Monitor
platformio device monitor -b 115200
```

### 4. Build 2.1 (Dual TAS5805M)
```bash
# Build
platformio run -e esp32-bt-speaker-2ch-1

# Upload
platformio run -t upload -e esp32-bt-speaker-2ch-1

# Monitor
platformio device monitor -b 115200
```

### 5. Pair Bluetooth
1. Tunggu ESP32 startup selesai (lihat serial monitor)
2. Buka Bluetooth Settings di smartphone/laptop
3. Cari device:
   - 2.0: `ESP32_BT_Speaker_2.0`
   - 2.1: `ESP32_BT_Speaker_2.1`
4. Pair & Connect
5. Play music 🎵

## 📊 Serial Monitor Output

### 2.0 System
```
====================================================
  ESP32 Bluetooth Speaker 2.0 + TAS5805M
  1x TAS5805M (Stereo L/R)
====================================================
[I2C_Main] Init SDA=21, SCL=22
[DAC_Main] Initializing TAS5805M (Stereo L/R)...
[DAC_Main] TAS5805M initialized
[DAC_Main] Set volume = 100
[DAC_Main] Set analog gain = MAX
[I2S] BCLK=26, LRCLK=25, DIN=14
[BT] Starting A2DP as ESP32_BT_Speaker_2.0

====================================================
[BT] Ready for pairing
Pair your phone and play music.
====================================================

[BT] Connected
[A2DP] Audio started
```

### 2.1 System
```
====================================================
  ESP32 Bluetooth Speaker 2.1 + 2x TAS5805M
  TAS5805M_Main: Stereo L/R
  TAS5805M_Sub: Mono Subwoofer
====================================================
[I2C_Main] Init SDA=21, SCL=22
[DAC_Main] Initializing TAS5805M (Stereo L/R)...
[DAC_Main] TAS5805M initialized
[DAC_Main] Set volume = 100
[DAC_Main] Set analog gain = MAX
[I2C_Sub] Init SDA=4, SCL=5
[DAC_Sub] Initializing TAS5805M (Mono Subwoofer)...
[DAC_Sub] TAS5805M initialized
[DAC_Sub] Set volume = 80
[DAC_Sub] Set analog gain = MAX
[I2S] BCLK=26, LRCLK=25, DIN=14
[BT] Starting A2DP as ESP32_BT_Speaker_2.1

====================================================
[BT] Ready for pairing
Pair your phone and play music.
====================================================

[BT] Connected
[A2DP] Audio started
```

## 🔧 Customization

### Change Bluetooth Name
Edit `platformio.ini`:
```ini
-D BT_SPEAKER_NAME="My_Speaker_Name"
```

### Change Pin Configuration
Edit `platformio.ini` untuk 2.0:
```ini
-D PIN_I2C_SDA=21
-D PIN_I2C_SCL=22
-D PIN_I2S_FS=25
-D PIN_I2S_SCK=26
-D PIN_I2S_SD=14
```

Edit `platformio.ini` untuk 2.1 (tambah I2C secondary):
```ini
-D PIN_I2C_SDA_SUB=4
-D PIN_I2C_SCL_SUB=5
```

### Change Volume
Edit `src/main.cpp`:

Untuk 2.0:
```cpp
uint8_t volume_main = 100;  // Range: 0-124
Tas5805m_Main.setVolume100(volume_main);
```

Untuk 2.1:
```cpp
uint8_t volume_main = 100;  // Main speaker
uint8_t volume_sub = 80;    // Subwoofer (biasanya lebih rendah)
```

### Change Gain
Edit `src/main.cpp`:

```cpp
// Untuk external 15V+ power (RECOMMENDED):
Tas5805m_Main.setAnalogGain(TAS5805M_MAX_GAIN);    // 0dB

// Untuk USB power only:
Tas5805m_Main.setAnalogGain(TAS5805M_MIN_GAIN);    // -15.5dB
```

## 📚 Project Structure

```
ESP32-BT-Speaker-TAS5805M/
├── platformio.ini          # Build configuration (2.0 & 2.1)
├── include/
│   └── config.h            # Pin definitions & macros
├── src/
│   └── main.cpp            # Main firmware (supports 2.0 & 2.1)
├── docs/
│   └── WIRING_2.1.md       # Detailed wiring for 2.1 system
├── schematic/
│   └── pinout.txt          # Pin reference
└── README.md               # This file
```

## 🐛 Troubleshooting

| Masalah | Solusi |
|--------|--------|
| Audio tidak keluar | Cek I2S wiring (25, 26, 14); Cek power TAS5805M |
| Bluetooth tidak terdeteksi | Reset ESP32; Cek Bluetooth aktif di device; Lihat serial monitor |
| Stereo kiri/kanan tidak keluar | Cek OUT_L/OUT_R wiring di TAS5805M |
| Subwoofer tidak keluar (2.1) | Cek I2C SDA(4), SCL(5); Cek power TAS5805M_Sub |
| I2C error | Cek wiring tidak tertukar; Pastikan pull-up resistor ada |
| Audio terlalu kecil | Gunakan external 15V+ power; Cek gain setting |
| Clipping audio | Turunkan volume atau gunakan MIN_GAIN |
| Build error | Update PlatformIO: `platformio upgrade`; Hapus folder `.pio` |

## 📖 References

- [TAS5805M Datasheet](https://www.ti.com/product/TAS5805M)
- [ESP32-A2DP Library](https://github.com/pschatzmann/ESP32-A2DP)
- [sonocotta/esp32-audio-dock](https://github.com/sonocotta/esp32-audio-dock)
- [ESP32 I2S Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html)

## 📄 License

MIT License - Feel free to use and modify

---

**Dibuat oleh**: tatankalan15-a11y  
**Repository**: [ESP32-BT-Speaker-TAS5805M](https://github.com/tatankalan15-a11y/ESP32-BT-Speaker-TAS5805M)  
**Status**: Ready for 2.0 & 2.1 systems
