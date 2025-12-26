# Physical Computing ESP32 + Next.js Dashboard

## Project Overview
ESP32 reads BME280 sensor (temp/humidity/pressure) over I2C, sends data to Next.js dashboard via HTTP, displays on SSD1306 OLED.

## Wiring (ESP32 DevKit)
- ESP32 3V3 -> BME280 VCC, SSD1306 VCC
- ESP32 GND -> BME280 GND, SSD1306 GND
- ESP32 GPIO21 (SDA) -> BME280 SDA, SSD1306 SDA
- ESP32 GPIO22 (SCL) -> BME280 SCL, SSD1306 SCL

## Setup
1. Install VS Code + PlatformIO extension.
2. For firmware: Open firmware/ in VS Code, build/upload.
3. For dashboard: cd dashboard && npm install && npm run dev.
4. Deploy dashboard to Vercel.

## Roadmap
- Week 1: Blink + serial, sensor read.
- Week 2: Wi-Fi send, dashboard, OTA.

## Demo
[Link to video/repo after shipping]
