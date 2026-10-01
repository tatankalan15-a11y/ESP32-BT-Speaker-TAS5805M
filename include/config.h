#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// Pin Configuration - ESP32 DevKit
// ============================================================================

// I2C Pins (for TAS5805M control via I2C)
#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 21  // GPIO 21
#endif

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 22  // GPIO 22
#endif

// I2S Pins (for audio data stream)
#ifndef PIN_I2S_FS
#define PIN_I2S_FS 25   // LRCLK / Frame Sync
#endif

#ifndef PIN_I2S_SCK
#define PIN_I2S_SCK 26  // BCLK / Bit Clock
#endif

#ifndef PIN_I2S_SD
#define PIN_I2S_SD 14   // DIN / Serial Data
#endif

// ============================================================================
// Bluetooth Configuration
// ============================================================================

#ifndef BT_SPEAKER_NAME
#define BT_SPEAKER_NAME "ESP32_BT_Speaker"
#endif

// ============================================================================
// Serial Configuration
// ============================================================================

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#endif // CONFIG_H
