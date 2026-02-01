#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

// Initialize OLED display
bool initDisplay();

// Update display with current information
void updateDisplay(uint8_t channel, int frequency, uint32_t packetsPerSec, int8_t rssi, bool muted);

// Show initialization screen
void showInitScreen();

// Show error message
void showError(const char* message);

#endif // DISPLAY_H
