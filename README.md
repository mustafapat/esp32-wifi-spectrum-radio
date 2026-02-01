# ESP32-S3 WiFi Spektrum Radyo

ESP32-S3 geliştirme kartı kullanarak Wi-Fi spektrumunu (2.4GHz bandı, kanal 1-13) bir radyo alıcısı gibi dinlemeyi sağlayan proje. Wi-Fi kanallarındaki veri trafiği, ham RF sinyali olarak yakalanıp ses dalgasına dönüştürülerek hoparlörden "hışırtı, cızırtı, dijital gürültü" şeklinde dinlenebilir.

## 🎯 Özellikler

- **Gerçek Zamanlı Wi-Fi Dinleme**: 2.4GHz Wi-Fi spektrumunda (kanal 1-13) promiscuous mode ile paket yakalama
- **Ses Çıkışı**: Wi-Fi trafiği gerçek zamanlı olarak ses dalgasına dönüştürülür
- **OLED Ekran**: Kanal, frekans, sinyal gücü ve paket istatistikleri görüntülenir
- **Rotary Encoder Kontrolü**: Kanal değiştirme ve ses kontrolü
- **Mute Fonksiyonu**: Encoder butonuna basarak sesi kapatabilirsiniz

## 🔧 Donanım Bileşenleri

| Bileşen | Açıklama |
|---------|----------|
| **ESP32-S3 DevKit** | Ana işlemci |
| **SSD1306 OLED (128x64)** | I2C ekran modülü |
| **Rotary Encoder** | Şaft butonlu encoder |
| **MAX98357A** | I2S dijital ses amplifikatörü |
| **Hoparlör** | 4-8Ω, 3W hoparlör |

## 📌 Pin Bağlantıları

### OLED Ekran (I2C)
```
┌─────────┬──────────┐
│ OLED    │ ESP32-S3 │
├─────────┼──────────┤
│ SDA     │ GPIO 21  │
│ SCL     │ GPIO 22  │
│ VCC     │ 3.3V     │
│ GND     │ GND      │
└─────────┴──────────┘
```

### Rotary Encoder
```
┌─────────┬──────────┐
│ Encoder │ ESP32-S3 │
├─────────┼──────────┤
│ CLK     │ GPIO 32  │
│ DT      │ GPIO 33  │
│ SW      │ GPIO 25  │
│ VCC     │ 3.3V     │
│ GND     │ GND      │
└─────────┴──────────┘
```

### MAX98357A (I2S Amplifikatör)
```
┌──────────┬──────────┐
│ MAX98357 │ ESP32-S3 │
├──────────┼──────────┤
│ BCLK     │ GPIO 26  │
│ LRC      │ GPIO 27  │
│ DIN      │ GPIO 14  │
│ VCC      │ 5V       │
│ GND      │ GND      │
└──────────┴──────────┘
```

### Bağlantı Şeması (ASCII Art)
```
                           ESP32-S3
                    ┌──────────────────┐
    OLED SDA ───────┤ GPIO21           │
    OLED SCL ───────┤ GPIO22           │
                    │                  │
    ENC CLK  ───────┤ GPIO32           │
    ENC DT   ───────┤ GPIO33           │
    ENC SW   ───────┤ GPIO25           │
                    │                  │
    I2S BCLK ───────┤ GPIO26           │
    I2S LRC  ───────┤ GPIO27           │
    I2S DIN  ───────┤ GPIO14           │
                    │                  │
    3.3V ───────────┤ 3.3V             │
    5V ─────────────┤ 5V               │
    GND ────────────┤ GND              │
                    └──────────────────┘
                            │
                            │ I2S
                            ↓
                    ┌──────────────┐
                    │  MAX98357A   │
                    │  Amplifier   │
                    └──────┬───────┘
                           │
                           ↓
                    ┌──────────────┐
                    │   Hoparlör   │
                    │   4-8Ω, 3W   │
                    └──────────────┘
```

## 📡 Wi-Fi Kanalları ve Frekanslar

| Kanal | Frekans (MHz) | Kullanım Alanı |
|-------|---------------|----------------|
| 1     | 2412          | Yaygın         |
| 2     | 2417          |                |
| 3     | 2422          |                |
| 4     | 2427          |                |
| 5     | 2432          |                |
| 6     | 2437          | Çok yaygın     |
| 7     | 2442          |                |
| 8     | 2447          |                |
| 9     | 2452          |                |
| 10    | 2457          |                |
| 11    | 2462          | Yaygın (ABD)   |
| 12    | 2467          |                |
| 13    | 2472          |                |

## 🚀 Kurulum

### Gereksinimler
- [PlatformIO](https://platformio.org/) (VS Code eklentisi veya CLI)
- USB kablo (ESP32-S3 programlama için)

### Adımlar

1. **Projeyi klonlayın**
   ```bash
   git clone https://github.com/mustafapat/esp32-wifi-spectrum-radio.git
   cd esp32-wifi-spectrum-radio
   ```

2. **Donanımı bağlayın**
   - Yukarıdaki pin bağlantıları tablosuna göre tüm bileşenleri bağlayın
   - USB ile ESP32-S3'ü bilgisayara bağlayın

3. **PlatformIO ile derleyin ve yükleyin**
   ```bash
   pio run --target upload
   ```
   
   Veya VS Code PlatformIO eklentisinde "Upload" butonuna tıklayın.

4. **Seri monitörü açın**
   ```bash
   pio device monitor
   ```

## 🎮 Kullanım

### Başlangıç
- Cihaz açıldığında OLED ekranda "WiFi Spektrum Radyo" başlangıç ekranı görünür
- Varsayılan olarak Kanal 6 (2437 MHz) seçilidir

### Kanal Değiştirme
- **Rotary encoder'ı sağa çevirin**: Bir sonraki kanala geçer (1→2→...→13→1)
- **Rotary encoder'ı sola çevirin**: Bir önceki kanala geçer (13→12→...→1→13)

### Ses Kontrolü
- **Encoder butonuna basın**: Sesi kapat/aç (MUTE/UNMUTE)
- Kapatıldığında ekranda "[MUTE]" görünür

### OLED Ekran Bilgileri
```
┌──────────────────────┐
│ WiFi Spektrum Radyo  │
├──────────────────────┤
│ Kanal: 6             │
│ Frekans: 2437 MHz    │
│                      │
│ Sinyal: ████████░░░  │
│ Paket/s: 127         │
│                      │
│ [MUTE]               │ (ses kapalıysa)
└──────────────────────┘
```

### Ne Duyarsınız?

- **Yoğun trafikte**: Sürekli cızırtı, hışırtı, dijital gürültü sesi
- **Az trafikte**: Ara sıra çıtırtı, kesik kesik sesler
- **Sessiz kanalda**: Hafif beyaz gürültü (arka plan sesi)
- **Büyük paketler**: Daha uzun ve yoğun gürültü
- **Küçük paketler**: Kısa, keskin çıtırtılar

Bu sesler gerçek Wi-Fi paketlerinin ham byte verilerinden üretilir - tıpkı eski dial-up modem sesi gibi!

## 🛠️ Teknik Detaylar

### Çalışma Prensibi

1. **Promiscuous Mode**: ESP32-S3 seçili Wi-Fi kanalındaki tüm paketleri yakalar
2. **Paket İşleme**: Gelen paket verileri (byte dizisi) ses örneklerine dönüştürülür
3. **Ses Çıkışı**: I2S protokolü ile MAX98357A'ya dijital ses gönderilir
4. **Amplifikasyon**: MAX98357A sesi yükselterek hoparlöre gönderir

### Ses Dönüşüm Algoritması

```cpp
// Her paket byte'ı 16-bit ses örneğine dönüştürülür
int16_t sample = ((int16_t)data[i] - 128) * 256;
sample = (sample * volume) / 100;
```

- 8-bit unsigned (0-255) → 16-bit signed (-32768 to +32767)
- Ses seviyesi ayarı uygulanır
- I2S buffer'a yazılır

### Proje Yapısı

```
esp32-wifi-spectrum-radio/
├── src/
│   ├── main.cpp              # Ana program döngüsü
│   ├── wifi_sniffer.cpp      # Promiscuous mode & paket yakalama
│   ├── wifi_sniffer.h
│   ├── audio_engine.cpp      # Ses dönüşümü & I2S çıkış
│   ├── audio_engine.h
│   ├── display.cpp           # SSD1306 OLED kontrolü
│   ├── display.h
│   ├── encoder.cpp           # Rotary encoder kontrolü
│   ├── encoder.h
│   └── config.h              # Pin tanımları & sabitler
├── platformio.ini            # PlatformIO yapılandırması
└── README.md                 # Bu dosya
```

### Kullanılan Kütüphaneler

- **Adafruit GFX Library**: Grafik temel fonksiyonları
- **Adafruit SSD1306**: OLED ekran sürücüsü
- **ESP32 I2S Driver**: I2S ses çıkışı
- **ESP32 WiFi API**: Promiscuous mode

## 🔍 Sorun Giderme

### Ekran çalışmıyor
- I2C bağlantılarını kontrol edin (SDA/SCL)
- OLED adresinin 0x3C olduğundan emin olun
- 3.3V besleme voltajını kontrol edin

### Ses çıkmıyor
- I2S pinlerini kontrol edin (BCLK, LRC, DIN)
- MAX98357A'nın 5V besleme aldığından emin olun
- Hoparlörün doğru bağlandığını kontrol edin
- Mute durumunu kontrol edin (encoder butonuna basın)

### Encoder çalışmıyor
- CLK, DT, SW pinlerini kontrol edin
- Pull-up dirençlerin aktif olduğundan emin olun
- Encoder kalitesini kontrol edin (bazı ucuz encoder'lar sorunludur)

### Paket yakalanmıyor
- Çevrenizdeki Wi-Fi trafiğini kontrol edin
- Farklı kanalları deneyin (6, 1, 11 en yaygın)
- ESP32'nin anten bağlantısını kontrol edin

## 📝 Notlar

- Bu proje **eğitim amaçlıdır** ve Wi-Fi spektrumunu dinleme/görselleştirme amacıyla yapılmıştır
- Paketlerin içeriği çözümlenmez, sadece ham byte verisi ses olarak çıkış verilir
- **Yasal Uyarı**: Wi-Fi paketlerini yakalamak bazı ülkelerde yasal kısıtlamalara tabi olabilir. Yerel yasalara uygun kullanın
- Proje şifrelenmemiş paketleri veya yayın paketlerini yakalar

## 🎨 Gelecek Geliştirmeler

- [ ] Ses seviyesi ayarı (encoder basılı tutarak çevirme)
- [ ] Spektrum analizörü (FFT) görüntüsü
- [ ] Farklı ses profilleri (sinus dalgası, kare dalga vb.)
- [ ] SD karta kayıt özelliği
- [ ] Web arayüzü (Wi-Fi AP mode)
- [ ] Bluetooth paket yakalama desteği

## 📄 Lisans

Bu proje MIT lisansı altında sunulmaktadır.

## 🙏 Teşekkürler

- ESP32 community
- Adafruit libraries
- PlatformIO team

## 📧 İletişim

Sorularınız için GitHub Issues kullanabilirsiniz.

---

**Keyifli dinlemeler! 📻🎶**