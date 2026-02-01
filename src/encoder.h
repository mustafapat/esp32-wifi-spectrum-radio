#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

// Initialize rotary encoder
void initEncoder();

// Check for encoder rotation
// Returns: -1 (CCW), 0 (no change), 1 (CW)
int8_t checkEncoderRotation();

// Check if button is pressed
bool isButtonPressed();

// Check if button is held down
bool isButtonHeld();

#endif // ENCODER_H
