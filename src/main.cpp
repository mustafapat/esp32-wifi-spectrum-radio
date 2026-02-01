#include <Arduino.h>
#include "config.h"
#include "wifi_sniffer.h"
#include "audio_engine.h"
#include "display.h"
#include "encoder.h"

// Current state
uint8_t currentChannel = DEFAULT_CHANNEL;
bool audioMuted = false;
unsigned long lastDisplayUpdate = 0;
unsigned long lastStatsUpdate = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n=== WiFi Spektrum Radyo ===");
    
    // Initialize display first to show status
    Serial.println("Display baslatiliyor...");
    if (!initDisplay()) {
        Serial.println("HATA: Display baslatilamadi!");
        while (1) delay(1000);
    }
    showInitScreen();
    delay(1000);
    
    // Initialize encoder
    Serial.println("Encoder baslatiliyor...");
    initEncoder();
    
    // Initialize audio
    Serial.println("Audio baslatiliyor...");
    if (!initAudio()) {
        Serial.println("HATA: Audio baslatilamadi!");
        showError("Audio baslatma hatasi");
        while (1) delay(1000);
    }
    
    // Initialize WiFi sniffer
    Serial.println("WiFi sniffer baslatiliyor...");
    initWiFiSniffer();
    setWiFiChannel(currentChannel);
    
    Serial.println("Sistem hazir!");
    Serial.print("Dinlenen kanal: ");
    Serial.println(currentChannel);
    
    lastDisplayUpdate = millis();
    lastStatsUpdate = millis();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Check encoder rotation for channel change
    int8_t rotation = checkEncoderRotation();
    if (rotation != 0) {
        int newChannel = currentChannel + rotation;
        
        // Wrap around channels
        if (newChannel < MIN_WIFI_CHANNEL) {
            newChannel = MAX_WIFI_CHANNEL;
        } else if (newChannel > MAX_WIFI_CHANNEL) {
            newChannel = MIN_WIFI_CHANNEL;
        }
        
        if (newChannel != currentChannel) {
            currentChannel = newChannel;
            setWiFiChannel(currentChannel);
            resetPacketCount();
            
            Serial.print("Kanal degisti: ");
            Serial.print(currentChannel);
            Serial.print(" (");
            Serial.print(CHANNEL_FREQ[currentChannel]);
            Serial.println(" MHz)");
            
            // Force immediate display update
            lastDisplayUpdate = 0;
        }
    }
    
    // Check button press for mute toggle
    if (isButtonPressed()) {
        audioMuted = !audioMuted;
        setMute(audioMuted);
        
        Serial.print("Audio: ");
        Serial.println(audioMuted ? "MUTED" : "UNMUTED");
        
        // Force immediate display update
        lastDisplayUpdate = 0;
    }
    
    // Update display periodically
    if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        uint32_t pps = getPacketsPerSecond();
        int8_t rssi = getAverageRSSI();
        // Bounds check before array access
        int frequency = (currentChannel >= 0 && currentChannel < 14) ? 
                        CHANNEL_FREQ[currentChannel] : 0;
        
        updateDisplay(currentChannel, frequency, pps, rssi, audioMuted);
        lastDisplayUpdate = currentTime;
    }
    
    // Print statistics periodically
    if (currentTime - lastStatsUpdate >= STATS_UPDATE_INTERVAL) {
        uint32_t pps = getPacketsPerSecond();
        Serial.print("Kanal ");
        Serial.print(currentChannel);
        Serial.print(": ");
        Serial.print(pps);
        Serial.print(" paket/s, RSSI: ");
        Serial.print(getAverageRSSI());
        Serial.println(" dBm");
        
        lastStatsUpdate = currentTime;
    }
    
    // Generate background noise if channel is quiet
    generateBackgroundNoise();
    
    // Small delay to prevent watchdog issues
    delay(10);
}
