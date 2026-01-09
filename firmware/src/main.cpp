#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid = "DEFAULT";
const char* password = "fiberpls123";
const char* serverUrl = "https://13ba19d0156730f7-103-40-2-54.serveousercontent.com/api/sensor";


#define LED_PIN 2
#define BME_SDA 21
#define BME_SCL 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BME280 bme; // I2C

void scanI2C() {
    byte error, address;
    int nDevices = 0;
    Serial.println("Scanning I2C bus...");
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0) Serial.println("No I2C devices found");
    else Serial.println("done");
}

void setup() {
    Serial.begin(9600);        // Initialize serial at 115200 baud

    // WiFi scan
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Scanning for WiFi networks...");
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("No networks found");
    } else {
        for (int i = 0; i < n; ++i) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(" dBm)");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " [Open]" : " [Encrypted]");
            delay(10);
        }
    }
    Serial.println();

    WiFi.begin(ssid, password);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.print("Connecting to WiFi...");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {  // Increased attempts
        delay(500);
        Serial.print(".");
        attempts++;
        if (attempts % 10 == 0) {
            Serial.print(" (");
            Serial.print(attempts);
            Serial.print("/40) ");
        }
    }

    Serial.print("\nFinal WiFi status: ");
    Serial.println(WiFi.status());
    Serial.print("WL_CONNECTED constant: ");
    Serial.println(WL_CONNECTED);

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal Strength: ");
        Serial.println(WiFi.RSSI());
    } else {
        Serial.println("\nWifi connection failed!");
    }

    pinMode(LED_PIN, OUTPUT); // Set GPIO 2 as output
    Wire.begin(BME_SDA, BME_SCL); // Start I2C with specified SDA and SCL pins

    scanI2C(); // Scan I2C bus for devices

    if (!bme.begin(0x76)) {
        Serial.println("BME280 not found!");
        while (1);
    }
    Serial.println("BME280 initialized.");

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 not found!");
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    Serial.println("SSD1306 initialized.");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);    // Turn LED on (3.3V)
    Serial.println("LED ON");

    float temp = bme.readTemperature();
    float hum = bme.readHumidity();
    float pres = bme.readPressure() / 100.0F;

    Serial.print("Temp: "); Serial.print(temp); Serial.println(" C");
    Serial.print("Hum: "); Serial.print(hum); Serial.println(" %");
    Serial.print("Pres: "); Serial.print(pres); Serial.println(" hPa");

    // Update OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("Temp : %.1f C\nHum  : %.1f %%\nPres : %.1f hPa", temp, hum, pres);
    display.display();

    if (WiFi.status() == WL_CONNECTED) {
        // HTTPClient httpTest;
        // httpTest.begin("https://httpbin.org/get");
        // int testCode = httpTest.GET();
        // Serial.print("Test GET Code: ");
        // Serial.println(testCode);
        // if (testCode > 0) {
        //     String testResponse = httpTest.getString();
        //     Serial.println("Test Response: " + testResponse.substring(0, 100));
        //     Serial.println("HTTP works!");
        // } else {
        //     Serial.println("HTTP test failed!");
        // }
        // httpTest.end();

        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");
        http.setTimeout(10000);  // 10 second timeout
        Serial.print("Attempting POST to: ");
        Serial.println(serverUrl);
        String payload = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + ",\"pres\":" + String(pres) + "}";
        Serial.print("Payload: ");
        Serial.println(payload);
        int httpResponseCode = http.POST(payload);
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 200) {
            String response = http.getString();
            Serial.println("Response: " + response);
            Serial.println("Data sent successfully!");
        } else if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("HTTP Error ");
            Serial.print(httpResponseCode);
            Serial.print(": ");
            Serial.println(response);
        } else {
            Serial.print("Connection Error: ");
            Serial.println(http.errorToString(httpResponseCode));
        }
        http.end();
    } else {
        Serial.println("WiFi not connected, skipping data send.");
    }

    delay(2000);                    // Wait 2 seconds
    
    digitalWrite(LED_PIN, LOW);     // Turn LED off (0V)
    Serial.println("LED OFF");
    delay(2000);                    // Wait 2 seconds
}