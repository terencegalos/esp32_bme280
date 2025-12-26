# Wiring Diagram

## ESP32 DevKit to BME280 + SSD1306 (I2C)

Use breadboard for solderless connections. ESP32 I2C pins: SDA=GPIO21, SCL=GPIO22.

ESP32 DevKit          BME280 Module          SSD1306 OLED
-------------         --------------         -------------
3V3 (3.3V)    ------> VCC                   VCC
GND          ------> GND                   GND
GPIO21 (SDA) ------> SDA                   SDA
GPIO22 (SCL) ------> SCL                   SCL

- Power ESP32 via USB cable to computer or 5V adapter.
- BME280 address: 0x76 or 0x77 (check module).
- SSD1306 address: 0x3C (default).

## Photos
[Add photos of your wired setup here after building.]
