#include <Arduino.h>
#include <Wire.h>
#include "config.h"

#include <BluetoothA2DPSink.h>

const char *TAG = "BT_SPEAKER";

// ============================================================================
// TAS5805M DAC instances
// ============================================================================
#ifdef CONFIG_DAC_TAS5805M
#include <tas5805m.hpp>

// Main amplifier (Stereo L/R)
tas5805m Tas5805m_Main(&Wire);

#if SPEAKER_MODE == 21
// Secondary I2C for subwoofer amplifier
TwoWire Wire_Sub(1);
tas5805m Tas5805m_Sub(&Wire_Sub);
#endif
#endif

// Bluetooth A2DP
BluetoothA2DPSink a2dp_sink;

// ============================================================================
// Bluetooth Callbacks
// ============================================================================
void on_connection_state_changed(esp_a2d_connection_state_t state, void *arg) {
    switch (state) {
        case ESP_A2D_CONNECTION_STATE_CONNECTED:
            Serial.println("[BT] Connected");
            break;

        case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
            Serial.println("[BT] Disconnected");
            break;

        default:
            Serial.printf("[BT] state=%d\n", state);
            break;
    }
}

void on_audio_state_changed(esp_a2d_audio_state_t state, void *arg) {
    switch (state) {
        case ESP_A2D_AUDIO_STATE_STARTED:
            Serial.println("[A2DP] Audio started");
            break;

        case ESP_A2D_AUDIO_STATE_STOPPED:
            Serial.println("[A2DP] Audio stopped");
            break;

        case ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND:
            Serial.println("[A2DP] Audio suspended");
            break;

        default:
            Serial.printf("[A2DP] state=%d\n", state);
            break;
    }
}

// ============================================================================
// Setup
// ============================================================================
void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);

    Serial.println("\n");
    Serial.println("====================================================");
#if SPEAKER_MODE == 2
    Serial.println("  ESP32 Bluetooth Speaker 2.0 + TAS5805M");
    Serial.println("  1x TAS5805M (Stereo L/R)");
#elif SPEAKER_MODE == 21
    Serial.println("  ESP32 Bluetooth Speaker 2.1 + 2x TAS5805M");
    Serial.println("  TAS5805M_Main: Stereo L/R");
    Serial.println("  TAS5805M_Sub: Mono Subwoofer");
#endif
    Serial.println("====================================================");

    // ========================================================================
    // Initialize Main I2C (for main amplifier stereo L/R)
    // ========================================================================
    Serial.printf("[I2C_Main] Init SDA=%d, SCL=%d\n", PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

#ifdef CONFIG_DAC_TAS5805M
    // Initialize Main TAS5805M (Stereo L/R)
    Serial.println("[DAC_Main] Initializing TAS5805M (Stereo L/R)...");
    if (Tas5805m_Main.init()) {
        Serial.println("[DAC_Main] TAS5805M init FAILED");
        while (1) delay(1000);
    }
    Serial.println("[DAC_Main] TAS5805M initialized");

    uint8_t volume_main = 100; // 0..124
    Serial.printf("[DAC_Main] Set volume = %d\n", volume_main);
    Tas5805m_Main.setVolume100(volume_main);

    Serial.println("[DAC_Main] Set analog gain = MAX");
    Tas5805m_Main.setAnalogGain(TAS5805M_MAX_GAIN);

#if SPEAKER_MODE == 21
    // ====================================================================
    // Initialize Secondary I2C (for subwoofer amplifier)
    // ====================================================================
    Serial.printf("[I2C_Sub] Init SDA=%d, SCL=%d\n", PIN_I2C_SDA_SUB, PIN_I2C_SCL_SUB);
    Wire_Sub.begin(PIN_I2C_SDA_SUB, PIN_I2C_SCL_SUB);

    // Initialize Subwoofer TAS5805M
    Serial.println("[DAC_Sub] Initializing TAS5805M (Mono Subwoofer)...");
    if (Tas5805m_Sub.init()) {
        Serial.println("[DAC_Sub] TAS5805M init FAILED");
        while (1) delay(1000);
    }
    Serial.println("[DAC_Sub] TAS5805M initialized");

    // Set subwoofer volume slightly lower than main
    uint8_t volume_sub = 80; // 0..124
    Serial.printf("[DAC_Sub] Set volume = %d\n", volume_sub);
    Tas5805m_Sub.setVolume100(volume_sub);

    Serial.println("[DAC_Sub] Set analog gain = MAX");
    Tas5805m_Sub.setAnalogGain(TAS5805M_MAX_GAIN);
#endif
#endif

    // ========================================================================
    // Initialize Bluetooth A2DP
    // ========================================================================
    Serial.printf("[I2S] BCLK=%d, LRCLK=%d, DIN=%d\n",
                  PIN_I2S_SCK, PIN_I2S_FS, PIN_I2S_SD);

    a2dp_sink.set_pin_config({
        .bck_io_num = PIN_I2S_SCK,
        .ws_io_num = PIN_I2S_FS,
        .data_out_num = PIN_I2S_SD,
        .data_in_num = I2S_PIN_NO_CHANGE,
        .mck_io_num = I2S_PIN_NO_CHANGE
    });

    a2dp_sink.set_on_connection_state_changed(on_connection_state_changed);
    a2dp_sink.set_on_audio_state_changed(on_audio_state_changed);

    Serial.printf("[BT] Starting A2DP as %s\n", BT_SPEAKER_NAME);
    if (!a2dp_sink.start(BT_SPEAKER_NAME)) {
        Serial.println("[BT] Failed to start A2DP");
        while (1) delay(1000);
    }

    Serial.println("\n====================================================");
    Serial.println("[BT] Ready for pairing");
    Serial.println("Pair your phone and play music.");
    Serial.println("====================================================");
}

// ============================================================================
// Loop
// ============================================================================
void loop() {
    delay(1000);
}
