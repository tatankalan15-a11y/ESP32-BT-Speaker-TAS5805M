#ifndef CONFIG_H
#define CONFIG_H

// Pin Configuration
#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 21
#endif

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 22
#endif

#ifndef PIN_I2C_SDA_SUB
#define PIN_I2C_SDA_SUB 4   // Secondary I2C for subwoofer TAS5805M
#endif

#ifndef PIN_I2C_SCL_SUB
#define PIN_I2C_SCL_SUB 5   // Secondary I2C for subwoofer TAS5805M
#endif

#ifndef PIN_I2S_FS
#define PIN_I2S_FS 25       // LRCLK / Frame Sync
#endif

#ifndef PIN_I2S_SCK
#define PIN_I2S_SCK 26      // BCLK / Bit Clock
#endif

#ifndef PIN_I2S_SD
#define PIN_I2S_SD 14       // DIN / Serial Data
#endif

#ifndef BT_SPEAKER_NAME
#define BT_SPEAKER_NAME "ESP32_BT_Speaker"
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

// Speaker Mode
// 2: 2.0 stereo (1 TAS5805M)
// 21: 2.1 stereo + mono subwoofer (2 TAS5805M)
#ifndef SPEAKER_MODE
#define SPEAKER_MODE 2
#endif

// Subwoofer crossover frequency (Hz)
#ifndef SUBWOOFER_CUTOFF_HZ
#define SUBWOOFER_CUTOFF_HZ 150
#endif

#endif
