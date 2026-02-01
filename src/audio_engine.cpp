#include "audio_engine.h"
#include "config.h"
#include "driver/i2s.h"
#include <Arduino.h>

static bool audioMuted = false;
static uint8_t audioVolume = DEFAULT_VOLUME;
static unsigned long lastPacketTime = 0;
static const unsigned long NOISE_TIMEOUT = 500; // ms

bool initAudio() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = AUDIO_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = AUDIO_BUFFER_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK_PIN,
        .ws_io_num = I2S_LRC_PIN,
        .data_out_num = I2S_DIN_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    
    esp_err_t result = i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    if (result != ESP_OK) {
        return false;
    }
    
    result = i2s_set_pin(I2S_NUM, &pin_config);
    if (result != ESP_OK) {
        return false;
    }
    
    i2s_zero_dma_buffer(I2S_NUM);
    
    lastPacketTime = millis();
    return true;
}

void packetToAudio(const uint8_t *data, int length) {
    if (audioMuted || length <= 0) {
        return;
    }
    
    lastPacketTime = millis();
    
    // Convert packet bytes to 16-bit audio samples
    int16_t audioBuffer[AUDIO_BUFFER_SIZE];
    int sampleCount = 0;
    
    for (int i = 0; i < length && sampleCount < AUDIO_BUFFER_SIZE; i++) {
        // Convert 8-bit unsigned to 16-bit signed audio sample
        // Apply volume scaling
        int16_t sample = ((int16_t)data[i] - 128) * 256;
        sample = (sample * audioVolume) / 100;
        
        audioBuffer[sampleCount++] = sample;
        
        // Add some processing for better audio representation
        // Every 4th byte, add a slightly modified duplicate for texture
        if (i % 4 == 0 && sampleCount < AUDIO_BUFFER_SIZE) {
            int32_t modSample = (int32_t)sample + random(-1000, 1000);
            // Clamp to int16_t range to prevent overflow
            if (modSample > 32767) modSample = 32767;
            if (modSample < -32768) modSample = -32768;
            audioBuffer[sampleCount++] = (int16_t)modSample;
        }
    }
    
    // Write to I2S
    size_t bytesWritten;
    i2s_write(I2S_NUM, audioBuffer, sampleCount * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
}

void generateBackgroundNoise() {
    if (audioMuted) {
        return;
    }
    
    // Only generate noise if no packets received recently
    if (millis() - lastPacketTime < NOISE_TIMEOUT) {
        return;
    }
    
    // Generate low-level white noise
    int16_t noiseBuffer[64];
    for (int i = 0; i < 64; i++) {
        int16_t noise = random(-500, 500);
        noise = (noise * audioVolume) / 200; // Half volume for background
        noiseBuffer[i] = noise;
    }
    
    size_t bytesWritten;
    i2s_write(I2S_NUM, noiseBuffer, sizeof(noiseBuffer), &bytesWritten, 0);
}

void setMute(bool muted) {
    audioMuted = muted;
    if (muted) {
        i2s_zero_dma_buffer(I2S_NUM);
    }
}

bool isMuted() {
    return audioMuted;
}

void setVolume(uint8_t volume) {
    if (volume > MAX_VOLUME) {
        volume = MAX_VOLUME;
    }
    audioVolume = volume;
}

uint8_t getVolume() {
    return audioVolume;
}
