#ifndef WIFI_SNIFFER_H
#define WIFI_SNIFFER_H

#include <Arduino.h>

// Initialize WiFi sniffer in promiscuous mode
void initWiFiSniffer();

// Set the WiFi channel to listen on
void setWiFiChannel(uint8_t channel);

// Get current WiFi channel
uint8_t getCurrentChannel();

// Get packet statistics
uint32_t getPacketCount();
uint32_t getPacketsPerSecond();
void resetPacketCount();

// Get signal strength (RSSI)
int8_t getAverageRSSI();

#endif // WIFI_SNIFFER_H
