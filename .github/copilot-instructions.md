# AI Coding Agent Instructions for Physical Computing Project

## Architecture Overview
This is an IoT sensor dashboard system with three components:
- **Firmware** (`firmware/`): ESP32 microcontroller reads BME280 sensor (temperature, humidity, pressure) and sends data via HTTP POST to the server. Displays readings on SSD1306 OLED. See `firmware/src/main.cpp` for the main loop.
- **Dashboard Server** (`dashboard/`): Node.js Express server on port 3030. Stores latest sensor data in memory. Endpoints: POST/GET `/api/sensor`. See `dashboard/server.js`.
- **Client** (`client/`): React app fetches sensor data from server every 5 seconds and displays it. See `client/src/App.js`.

Data flow: ESP32 → HTTP POST → Server (in-memory storage) → Client fetches via GET.

## Development Workflows
- **Start Server**: `cd dashboard && npm run start` (runs `node server.js`)
- **Start Client**: `cd client && npm start` (opens http://localhost:3000)
- **Build Firmware**: `cd firmware && pio run -t upload` (compiles and uploads to ESP32)
- **Monitor Firmware**: `cd firmware && pio device monitor` (serial output at 9600 baud)
- **Tunneling for Remote Access**: Use Serveo SSH tunnel: `ssh -R 80:localhost:3030 serveo.net` to get public URL, then update URLs in `firmware/src/main.cpp` and `client/src/App.js`

## Project Conventions
- **URLs**: Hardcode server URLs in both firmware and client for tunneling (e.g., `http://[serveo-url]/api/sensor`). Comment out localhost versions.
- **Data Format**: JSON with keys `temp`, `hum`, `pres` (float values).
- **WiFi Credentials**: Hardcoded in firmware (`ssid`, `password`).
- **Sensor Pins**: BME280 on I2C SDA=21, SCL=22; OLED address 0x3C.
- **Timing**: Firmware sends data every 4 seconds; client polls every 5 seconds.
- **Dependencies**: Firmware uses Adafruit libraries; server uses Express + CORS; client is standard CRA.

## Integration Patterns
- **Cross-Component Communication**: HTTP only; no WebSockets or MQTT.
- **Error Handling**: Basic try/catch in client; firmware checks WiFi and HTTP response codes.
- **Debugging**: Serial prints in firmware; console logs in client/server.
- **Deployment**: Client builds with `npm run build`; server runs with Node; firmware uploads via PlatformIO.

## Key Files
- `TROUBLESHOOTING_GUIDE.md`: Detailed setup for tunneling and connection issues.
- `firmware/platformio.ini`: Board config (esp32dev), libraries, upload port (/dev/ttyUSB0).
- `dashboard/package.json`: Scripts and Express deps.
- `client/package.json`: React deps with testing library.</content>
<parameter name="filePath">/root/projects/physical_computing/.github/copilot-instructions.md