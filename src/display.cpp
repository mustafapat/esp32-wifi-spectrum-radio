#include "display.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

bool initDisplay() {
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        return false;
    }
    
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.display();
    
    return true;
}

void showInitScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("WiFi Spektrum"));
    display.println(F("Radyo"));
    display.println();
    display.println(F("Baslatiliyor..."));
    display.display();
}

void showError(const char* message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("HATA:"));
    display.println();
    display.println(message);
    display.display();
}

void updateDisplay(uint8_t channel, int frequency, uint32_t packetsPerSec, int8_t rssi, bool muted) {
    display.clearDisplay();
    
    // Title
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("WiFi Spektrum Radyo"));
    
    // Draw horizontal line
    display.drawLine(0, 10, OLED_WIDTH, 10, SSD1306_WHITE);
    
    // Channel info
    display.setCursor(0, 14);
    display.print(F("Kanal: "));
    display.println(channel);
    
    display.setCursor(0, 24);
    display.print(F("Frekans: "));
    display.print(frequency);
    display.println(F(" MHz"));
    
    // Signal strength bar
    display.setCursor(0, 36);
    display.print(F("Sinyal: "));
    
    // Convert RSSI to bar level (0-10)
    // RSSI typically ranges from -100 (weak) to -30 (strong)
    int barLevel = map(rssi, -100, -30, 0, 10);
    if (barLevel < 0) barLevel = 0;
    if (barLevel > 10) barLevel = 10;
    
    int barX = 50;
    for (int i = 0; i < 10; i++) {
        if (i < barLevel) {
            display.fillRect(barX + i * 7, 36, 5, 7, SSD1306_WHITE);
        } else {
            display.drawRect(barX + i * 7, 36, 5, 7, SSD1306_WHITE);
        }
    }
    
    // Packet rate
    display.setCursor(0, 46);
    display.print(F("Paket/s: "));
    display.println(packetsPerSec);
    
    // Mute status
    if (muted) {
        display.setCursor(0, 56);
        display.print(F("[MUTE]"));
    }
    
    display.display();
}
