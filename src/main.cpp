#include <Arduino.h>
#include <Wire.h>
#include "config.h"

#include <BluetoothA2DPSink.h>

const char *TAG = "BT_SPEAKER";

// ============================================================================
// TAS5805M DAC Instance
// ============================================================================
#ifdef CONFIG_DAC_TAS5805M
#include <tas5805m.hpp>
tas5805m Tas5805m(&Wire);
#endif

// ============================================================================
// Bluetooth A2DP Sink
// ============================================================================
BluetoothA2DPSink a2dp_sink;

// ============================================================================
// Bluetooth Callbacks
// ============================================================================

void on_connection_state_changed(esp_a2d_connection_state_t state, void *arg) {
    switch (state) {
        case ESP_A2D_CONNECTION_STATE_CONNECTED:
            Serial.println("[BT] Device Connected");
            break;

        case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
            Serial.println("[BT] Device Disconnected");
            break;

        default:
            Serial.printf("[BT] Connection state: %d\n", state);
            break;
    }
}

void on_audio_state_changed(esp_a2d_audio_state_t state, void *arg) {
    switch (state) {
        case ESP_A2D_AUDIO_STATE_STARTED:
            Serial.println("[AUDIO] Playback Started");
            break;

        case ESP_A2D_AUDIO_STATE_STOPPED:
            Serial.println("[AUDIO] Playback Stopped");
            break;

        case ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND:
            Serial.println("[AUDIO] Playback Paused");
            break;

        default:
            Serial.printf("[AUDIO] Audio state: %d\n", state);
            break;
    }
}

// ============================================================================
// Setup
// ============================================================================
void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);

    Serial.println("\n\n");
    Serial.println("╔═══════════════════════════════════════════════════════╗");
    Serial.println("║     ESP32 Bluetooth A2DP Speaker + TAS5805M DAC       ║");
    Serial.println("║            2.0 Stereo (Single Channel)                ║");
    Serial.println("╚═══════════════════════════════════════════════════════╝");
    Serial.println("");

    // ========================================================================
    // Initialize I2C Bus
    // ========================================================================
    Serial.printf("[I2C] Initializing... SDA=%d, SCL=%d\n", PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    delay(100);
    Serial.println("[I2C] Initialized");

#ifdef CONFIG_DAC_TAS5805M
    // ====================================================================
    // Initialize TAS5805M DAC
    // ====================================================================
    Serial.println("\n[DAC] Initializing TAS5805M...");
    if (Tas5805m.init()) {
        Serial.println("[DAC] ❌ FAILED - Check I2C connection (SDA=21, SCL=22)");
        Serial.println("[DAC] ❌ Possible issues:");
        Serial.println("       - I2C wiring not connected");
        Serial.println("       - TAS5805M power supply missing");
        Serial.println("       - I2C pull-up resistors missing");
        while (1) {
            delay(1000);
            Serial.print(".");
        }
    }
    Serial.println("[DAC] ✓ Initialized successfully");

    // Set Volume (0-124, where 100 is max without clipping)
    uint8_t volume = 100;
    Serial.printf("[DAC] Setting volume to %d/124\n", volume);
    if (Tas5805m.setVolume100(volume) != ESP_OK) {
        Serial.println("[DAC] ⚠ Warning: Volume setting may have failed");
    } else {
        Serial.println("[DAC] ✓ Volume set successfully");
    }

    // Set Analog Gain
    // For external 15V+ power: use TAS5805M_MAX_GAIN (0dB)
    // For USB power only: use TAS5805M_MIN_GAIN (-15.5dB) to avoid clipping
    Serial.println("[DAC] Setting analog gain to MAX (0dB)");
    Serial.println("[DAC] ⚠ Note: If audio clips, use TAS5805M_MIN_GAIN instead");
    if (Tas5805m.setAnalogGain(TAS5805M_MAX_GAIN) != ESP_OK) {
        Serial.println("[DAC] ⚠ Warning: Gain setting may have failed");
    } else {
        Serial.println("[DAC] ✓ Gain set successfully");
    }
#endif

    // ========================================================================
    // Configure I2S Audio Output
    // ========================================================================
    Serial.println("\n[I2S] Configuring audio output pins...");
    Serial.printf("[I2S] BCLK=%d, LRCLK=%d, DIN=%d\n", PIN_I2S_SCK, PIN_I2S_FS, PIN_I2S_SD);

    a2dp_sink.set_pin_config({
        .bck_io_num = PIN_I2S_SCK,              // BCLK / Bit Clock
        .ws_io_num = PIN_I2S_FS,                // LRCLK / Frame Sync
        .data_out_num = PIN_I2S_SD,             // DIN / Serial Data
        .data_in_num = I2S_PIN_NO_CHANGE,
        .mck_io_num = I2S_PIN_NO_CHANGE
    });
    Serial.println("[I2S] ✓ I2S configured");

    // ========================================================================
    // Setup Bluetooth A2DP Sink Callbacks
    // ========================================================================
    Serial.println("\n[BT] Setting up Bluetooth A2DP callbacks...");
    a2dp_sink.set_on_connection_state_changed(on_connection_state_changed);
    a2dp_sink.set_on_audio_state_changed(on_audio_state_changed);
    Serial.println("[BT] ✓ Callbacks registered");

    // ========================================================================
    // Start Bluetooth A2DP Sink
    // ========================================================================
    Serial.println("\n[BT] Starting Bluetooth A2DP Sink...");
    Serial.printf("[BT] Device Name: %s\n", BT_SPEAKER_NAME);

    if (!a2dp_sink.start(BT_SPEAKER_NAME)) {
        Serial.println("[BT] ❌ FAILED - Could not start Bluetooth A2DP");
        while (1) {
            delay(1000);
            Serial.print(".");
        }
    }
    Serial.println("[BT] ✓ Bluetooth A2DP started");

    // ========================================================================
    // Setup Complete - Ready for Connection
    // ========================================================================
    Serial.println("\n╔═══════════════════════════════════════════════════════╗");
    Serial.println("║               Setup Complete - Ready!                  ║");
    Serial.println("╠═══════════════════════════════════════════════════════╣");
    Serial.println("║ Next Steps:                                           ║");
    Serial.println("║ 1. Search for device on your phone/laptop             ║");
    Serial.println("║ 2. Look for: ESP32_BT_Speaker                         ║");
    Serial.println("║ 3. Pair and Connect                                   ║");
    Serial.println("║ 4. Play music from any audio app                      ║");
    Serial.println("║                                                       ║");
    Serial.println("║ Hardware Configuration:                               ║");
    Serial.println("║ • I2C: SDA=GPIO21, SCL=GPIO22                         ║");
    Serial.println("║ • I2S: BCLK=GPIO26, LRCLK=GPIO25, DIN=GPIO14          ║");
    Serial.println("║ • Speaker: Connected to TAS5805M OUT_L & OUT_R        ║");
    Serial.println("║ • Power: 5V USB (ESP32) + 15V (TAS5805M recommended)  ║");
    Serial.println("╚═══════════════════════════════════════════════════════╝");
    Serial.println("");
}

// ============================================================================
// Main Loop
// ============================================================================
void loop() {
    // Bluetooth A2DP sink runs in background
    // Keep processor running
    delay(1000);
}
