#ifndef CONFIG_H
#define CONFIG_H

// ===== Pin Definitions =====

// OLED Display (I2C)
#define OLED_SDA_PIN 21
#define OLED_SCL_PIN 22
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Rotary Encoder
#define ENCODER_CLK_PIN 32
#define ENCODER_DT_PIN 33
#define ENCODER_SW_PIN 25

// MAX98357A I2S Audio
#define I2S_BCLK_PIN 26
#define I2S_LRC_PIN 27
#define I2S_DIN_PIN 14
#define I2S_NUM I2S_NUM_0

// ===== Wi-Fi Channel Configuration =====
#define MIN_WIFI_CHANNEL 1
#define MAX_WIFI_CHANNEL 13
#define DEFAULT_CHANNEL 6

// Channel to Frequency mapping (MHz)
const int CHANNEL_FREQ[14] = {
    0,    // Channel 0 (not used)
    2412, // Channel 1
    2417, // Channel 2
    2422, // Channel 3
    2427, // Channel 4
    2432, // Channel 5
    2437, // Channel 6
    2442, // Channel 7
    2447, // Channel 8
    2452, // Channel 9
    2457, // Channel 10
    2462, // Channel 11
    2467, // Channel 12
    2472  // Channel 13
};

// ===== Audio Configuration =====
#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_BITS_PER_SAMPLE 16
#define AUDIO_CHANNELS 1
#define AUDIO_BUFFER_SIZE 512
#define MAX_VOLUME 100
#define DEFAULT_VOLUME 50

// ===== Display Update Configuration =====
#define DISPLAY_UPDATE_INTERVAL 100  // ms
#define STATS_UPDATE_INTERVAL 1000   // ms

// ===== Encoder Configuration =====
#define ENCODER_DEBOUNCE_DELAY 50    // ms

#endif // CONFIG_H
