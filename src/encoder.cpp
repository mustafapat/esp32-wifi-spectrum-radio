#include "encoder.h"
#include "config.h"
#include <Arduino.h>

static int encoderPos = 0;
static int lastEncoderPos = 0;
static uint8_t lastCLK = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long buttonPressTime = 0;
static bool buttonPressed = false;
static bool lastButtonState = HIGH;

void initEncoder() {
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
    
    lastCLK = digitalRead(ENCODER_CLK_PIN);
    lastButtonState = digitalRead(ENCODER_SW_PIN);
}

int8_t checkEncoderRotation() {
    uint8_t clk = digitalRead(ENCODER_CLK_PIN);
    uint8_t dt = digitalRead(ENCODER_DT_PIN);
    
    int8_t rotation = 0;
    
    if (clk != lastCLK) {
        unsigned long currentTime = millis();
        if (currentTime - lastDebounceTime > 5) { // 5ms debounce
            if (dt != clk) {
                encoderPos++;
                rotation = 1;
            } else {
                encoderPos--;
                rotation = -1;
            }
            lastDebounceTime = currentTime;
        }
    }
    
    lastCLK = clk;
    return rotation;
}

bool isButtonPressed() {
    uint8_t buttonState = digitalRead(ENCODER_SW_PIN);
    unsigned long currentTime = millis();
    
    // Detect button press (transition from HIGH to LOW)
    if (buttonState == LOW && lastButtonState == HIGH) {
        if (currentTime - lastDebounceTime > ENCODER_DEBOUNCE_DELAY) {
            lastDebounceTime = currentTime;
            buttonPressTime = currentTime;
            buttonPressed = true;
            lastButtonState = buttonState;
            return true;
        }
    } else if (buttonState == HIGH && lastButtonState == LOW) {
        buttonPressed = false;
        lastButtonState = buttonState;
    }
    
    lastButtonState = buttonState;
    return false;
}

bool isButtonHeld() {
    if (!buttonPressed) {
        return false;
    }
    
    unsigned long currentTime = millis();
    return (currentTime - buttonPressTime > 1000); // 1 second hold
}
