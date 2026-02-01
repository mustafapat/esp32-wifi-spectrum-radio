#include "wifi_sniffer.h"
#include "config.h"
#include "audio_engine.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <WiFi.h>

// Statistics
static uint32_t packetCount = 0;
static uint32_t packetCountLastSec = 0;
static uint32_t packetsPerSecond = 0;
static unsigned long lastSecondTime = 0;
static uint8_t currentChannel = DEFAULT_CHANNEL;
static int32_t rssiSum = 0;
static uint32_t rssiCount = 0;

// Promiscuous mode callback
void IRAM_ATTR wifiSnifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT && type != WIFI_PKT_DATA && type != WIFI_PKT_CTRL) {
        return;
    }
    
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    
    // Update statistics
    packetCount++;
    packetCountLastSec++;
    
    // Track RSSI
    rssiSum += pkt->rx_ctrl.rssi;
    rssiCount++;
    
    // Convert packet to audio
    uint8_t *payload = pkt->payload;
    int len = pkt->rx_ctrl.sig_len;
    
    // Limit length to avoid buffer overflow
    if (len > 128) len = 128;
    
    packetToAudio(payload, len);
}

void initWiFiSniffer() {
    // Initialize WiFi in station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    // Enable promiscuous mode
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&wifiSnifferCallback);
    
    // Set initial channel
    setWiFiChannel(currentChannel);
    
    lastSecondTime = millis();
}

void setWiFiChannel(uint8_t channel) {
    if (channel < MIN_WIFI_CHANNEL || channel > MAX_WIFI_CHANNEL) {
        return;
    }
    
    currentChannel = channel;
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    
    // Reset statistics when changing channel
    rssiSum = 0;
    rssiCount = 0;
}

uint8_t getCurrentChannel() {
    return currentChannel;
}

uint32_t getPacketCount() {
    return packetCount;
}

uint32_t getPacketsPerSecond() {
    unsigned long currentTime = millis();
    if (currentTime - lastSecondTime >= 1000) {
        packetsPerSecond = packetCountLastSec;
        packetCountLastSec = 0;
        lastSecondTime = currentTime;
    }
    return packetsPerSecond;
}

void resetPacketCount() {
    packetCount = 0;
    packetCountLastSec = 0;
    packetsPerSecond = 0;
}

int8_t getAverageRSSI() {
    if (rssiCount == 0) return -100;
    // Proper rounding for integer division
    int8_t avgRSSI = (rssiSum + rssiCount/2) / rssiCount;
    
    // Reset for next period
    rssiSum = 0;
    rssiCount = 0;
    
    return avgRSSI;
}
