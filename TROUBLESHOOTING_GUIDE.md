# ESP32 to Dashboard Server Connection Setup Guide

## Overview
This guide documents the steps to resolve the "connection refused" error when an ESP32 microcontroller tries to send sensor data (temperature, humidity, pressure) to a local Node.js dashboard server. The issue was caused by network segmentation and SSL complications. The final solution uses Serveo tunneling for reliable HTTP connections.

## Problem Summary
- ESP32 firmware was configured to send POST requests to `http://192.168.1.3:3030/api/sensor`.
- Server was running on `172.25.156.231:3030`, but ESP32 was on a different network segment (WiFi vs. Ethernet).
- Initial attempts with direct IP failed due to segmentation.
- Ngrok tunneling introduced SSL handshake issues with the ESP32.
- Switched to Serveo for HTTP-only tunneling, which worked reliably.

## Prerequisites
- ESP32 with BME280 sensor and OLED display.
- Node.js server (`dashboard/server.js`) listening on port 3030.
- PlatformIO installed for firmware compilation/upload.
- SSH available for tunneling.

## Working Steps

### 1. Start the Node.js Server
Ensure the dashboard server is running in the background:
```
cd dashboard && node server.js > server.log 2>&1 &
```

### 2. Set Up Serveo Tunneling
Create a reverse tunnel to expose port 3030 to the internet:
```
nohup ssh -R 80:localhost:3030 serveo.net -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null > serveo.log 2>&1 &
```

### 3. Retrieve the Public URL
Wait for the tunnel to initialize, then check the log for the URL:
```
sleep 5 && cat serveo.log
```
- Look for output like: `Forwarding HTTP traffic from http://[random-subdomain].serveousercontent.com`
- Copy the URL (e.g., `http://4a820f57e18cfce2-103-40-2-54.serveousercontent.com`)

### 4. Test the Tunnel
Verify the public URL reaches your server:
```
curl -I http://[your-serveo-url]/api/sensor
```
- Expected: HTTP 200 OK response.

### 5. Update Firmware Code
Edit `firmware/src/main.cpp`:
- Change `serverUrl` to the Serveo URL: `const char* serverUrl = "http://[your-serveo-url]/api/sensor";`
- Ensure simple HTTP usage: `HTTPClient http; http.begin(serverUrl);`

### 6. Upload Firmware and Monitor
Compile, upload, and monitor the ESP32:
```
source venv/bin/activate && cd firmware && pio run -t upload && pio device monitor
```
- Look for `HTTP Response Code: 200` and `Data sent successfully!` in the serial output.

## Troubleshooting Tips
- **Tunnel Fails**: Check `serveo.log` for errors. Ensure port 3030 is open (`sudo ufw allow 3030`).
- **ESP32 Can't Connect**: Confirm WiFi credentials and URL in firmware. Test with `curl` from another device.
- **Server Issues**: Check `server.log` for crashes. Restart with `node server.js`.
- **URL Changes**: Serveo URLs are random per session—update firmware each time.
- **Firewall**: Disable or configure firewall to allow port 3030.

## Why This Worked
- Serveo provides HTTP tunneling without SSL, avoiding ESP32 handshake failures.
- Bypasses network segmentation by routing through the internet.
- Simple, free, and requires no accounts.

## Next Steps
- For production, consider MQTT or cloud services (e.g., AWS IoT) for better reliability.
- Automate tunnel restarts with scripts.

This setup successfully sends sensor data from ESP32 to the dashboard server.