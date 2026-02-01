#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <Arduino.h>

// Initialize I2S audio output
bool initAudio();

// Process packet data to audio samples
void packetToAudio(const uint8_t *data, int length);

// Mute/unmute audio
void setMute(bool muted);
bool isMuted();

// Volume control (0-100)
void setVolume(uint8_t volume);
uint8_t getVolume();

// Generate background noise when no packets
void generateBackgroundNoise();

#endif // AUDIO_ENGINE_H
