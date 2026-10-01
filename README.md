# ESP32 Bluetooth Speaker 2.0 dengan TAS5805M

**Production Ready Firmware** untuk ESP32 Bluetooth Audio Receiver dengan Class-D Amplifier TAS5805M.

## 📋 Spesifikasi

| Aspek | Detail |
|-------|--------|
| **Platform** | ESP32 DevKit |
| **Amplifier** | 1x TAS5805M (Stereo L/R) |
| **Koneksi Audio** | Bluetooth A2DP |
| **Output Audio** | I2S ke TAS5805M |
| **Kontrol DAC** | I2C |
| **Speaker** | 2-10W Stereo |
| **Power** | 5V USB (ESP32) + 15V+ (TAS5805M) |
| **Status** | ✓ Production Ready |

## 🎯 Fitur

- ✅ Bluetooth A2DP Audio Receiver (Wireless Speaker)
- ✅ I2S Audio Output ke TAS5805M
- ✅ Automatic Volume Control (0-124)
- ✅ Real-time Connection Status
- ✅ Serial Monitor Diagnostics
- ✅ Multiple DSP Profiles (Stereo, Stereo+Loudness)

## 🔌 Hardware Wiring

### ESP32 DevKit → TAS5805M

```
I2C Control Bus:
  GPIO 21 (SDA) ──────→ TAS5805M SDA
  GPIO 22 (SCL) ──────→ TAS5805M SCL
  GND ─────────────────→ TAS5805M GND

I2S Audio Bus:
  GPIO 25 (LRCLK/WS) ──→ TAS5805M WS
  GPIO 26 (BCLK/SCK) ──→ TAS5805M SCK
  GPIO 14 (DIN/SD) ────→ TAS5805M DIN
  GND ─────────────────→ TAS5805M GND

Power:
  5V USB ──→ ESP32 VCC
  15V+ ────→ TAS5805M VCC (external power recommended)
  GND ─────→ Common Ground

Speaker Output:
  TAS5805M OUT_L ──→ Speaker Left Channel
  TAS5805M OUT_R ──→ Speaker Right Channel
```

## 🚀 Quick Start

### 1. Clone Repository
```bash
git clone https://github.com/tatankalan15-a11y/ESP32-BT-Speaker-TAS5805M.git
cd ESP32-BT-Speaker-TAS5805M
```

### 2. Install Dependencies
```bash
pip install platformio
```

### 3. Build & Upload

**Standard 2.0 (Flat Response):**
```bash
platformio run -e esp32-bt-speaker-2ch
platformio run -t upload -e esp32-bt-speaker-2ch
```

**With Loudness Enhancement (Level 1):**
```bash
platformio run -e esp32-bt-speaker-2ch-loudness1
platformio run -t upload -e esp32-bt-speaker-2ch-loudness1
```

**With Loudness Enhancement (Level 2 - More Bass):**
```bash
platformio run -e esp32-bt-speaker-2ch-loudness2
platformio run -t upload -e esp32-bt-speaker-2ch-loudness2
```

**With Loudness Enhancement (Level 3 - Maximum Bass):**
```bash
platformio run -e esp32-bt-speaker-2ch-loudness3
platformio run -t upload -e esp32-bt-speaker-2ch-loudness3
```

### 4. Monitor Serial Output
```bash
platformio device monitor -b 115200
```

Output yang diharapkan:
```
╔═══════════════════════════════════════════════════════╗
║     ESP32 Bluetooth A2DP Speaker + TAS5805M DAC       ║
║            2.0 Stereo (Single Channel)                ║
╚═══════════════════════════════════════════════════════╝

[I2C] Initializing... SDA=21, SCL=22
[I2C] Initialized

[DAC] Initializing TAS5805M...
[DAC] ✓ Initialized successfully
[DAC] Setting volume to 100/124
[DAC] ✓ Volume set successfully
[DAC] Setting analog gain to MAX (0dB)
[DAC] ✓ Gain set successfully

[I2S] Configuring audio output pins...
[I2S] BCLK=26, LRCLK=25, DIN=14
[I2S] ✓ I2S configured

[BT] Setting up Bluetooth A2DP callbacks...
[BT] ✓ Callbacks registered

[BT] Starting Bluetooth A2DP Sink...
[BT] Device Name: ESP32_BT_Speaker
[BT] ✓ Bluetooth A2DP started

╔═══════════════════════════════════════════════════════╗
║               Setup Complete - Ready!                  ║
╠═══════════════════════════════════════════════════════╣
║ Next Steps:                                           ║
║ 1. Search for device on your phone/laptop             ║
║ 2. Look for: ESP32_BT_Speaker                         ║
║ 3. Pair and Connect                                   ║
║ 4. Play music from any audio app                      ║
╚═══════════════════════════════════════════════════════╝
```

### 5. Pair Bluetooth

**From Smartphone/Laptop:**
1. Go to Bluetooth Settings
2. Search for: **ESP32_BT_Speaker**
3. Select and Pair
4. Open music app and play 🎵

## 🎨 Environment Options

| Environment | Mode | Description |
|-------------|------|-------------|
| `esp32-bt-speaker-2ch` | Stereo Flat | Neutral sound profile |
| `esp32-bt-speaker-2ch-loudness1` | Stereo L1 | Light bass/treble boost |
| `esp32-bt-speaker-2ch-loudness2` | Stereo L2 | Medium bass/treble boost |
| `esp32-bt-speaker-2ch-loudness3` | Stereo L3 | Heavy bass/treble boost |

## ⚙️ Customization

### Change Bluetooth Device Name
Edit `platformio.ini`:
```ini
-D BT_SPEAKER_NAME="My_Custom_Speaker"
```

### Change Pin Configuration
Edit `platformio.ini`:
```ini
-D PIN_I2C_SDA=21    # I2C Data
-D PIN_I2C_SCL=22    # I2C Clock
-D PIN_I2S_FS=25     # I2S Frame Sync
-D PIN_I2S_SCK=26    # I2S Bit Clock
-D PIN_I2S_SD=14     # I2S Serial Data
```

### Change Default Volume
Edit `src/main.cpp`, find line ~140:
```cpp
uint8_t volume = 100;  // Change to 0-124
```

### Change Analog Gain
Edit `src/main.cpp`, find line ~150:
```cpp
// For external 15V+ power supply (RECOMMENDED):
Tas5805m.setAnalogGain(TAS5805M_MAX_GAIN);    // 0dB - Full power

// For USB power only (to avoid clipping):
Tas5805m.setAnalogGain(TAS5805M_MIN_GAIN);    // -15.5dB - Reduced power
```

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| **No audio output** | • Check I2S wiring (pins 14, 25, 26)<br>• Check TAS5805M power supply<br>• Verify speaker connections |
| **Bluetooth not found** | • Reset ESP32<br>• Check Bluetooth is enabled on device<br>• Look for "ESP32_BT_Speaker" in device list |
| **I2C initialization failed** | • Check SDA/SCL wiring (pins 21, 22)<br>• Ensure pull-up resistors present (4.7kΩ)<br>• Check TAS5805M is powered |
| **Audio too quiet** | • Increase volume in music app<br>• Check gain setting (use MAX_GAIN)<br>• Use external 15V+ power supply |
| **Audio clips/distorts** | • Reduce volume<br>• Use TAS5805M_MIN_GAIN instead of MAX_GAIN<br>• Check power supply voltage |
| **Build error** | • Run: `platformio run --target clean`<br>• Update: `platformio upgrade`<br>• Delete: `.pio` folder and rebuild |

## 📊 Project Structure

```
ESP32-BT-Speaker-TAS5805M/
├── platformio.ini              # Build configuration (4 variants)
├── include/
│   └── config.h               # Pin definitions
├── src/
│   └── main.cpp               # Main firmware (2.0 implementation)
├── docs/
│   └── WIRING_2.1.md          # Reference for future 2.1 implementation
├── schematic/
│   └── pinout.txt             # Hardware reference
└── README.md                  # This file
```

## 📚 References

- [TAS5805M Datasheet](https://www.ti.com/product/TAS5805M)
- [ESP32-A2DP Library](https://github.com/pschatzmann/ESP32-A2DP)
- [sonocotta/esp32-tas5805m-dac](https://github.com/sonocotta/esp32-tas5805m-dac)
- [ESP32 I2S Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html)

## 📄 License

MIT License - Feel free to use and modify

---

**Repository**: [ESP32-BT-Speaker-TAS5805M](https://github.com/tatankalan15-a11y/ESP32-BT-Speaker-TAS5805M)  
**Status**: ✓ Production Ready  
**Last Updated**: 2026-10-01  
**Supported Hardware**: ESP32 DevKit + TAS5805M  
