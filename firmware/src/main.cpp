#include <Arduino.h>

// Blink example: LED on GPIO2 (built-in on ESP32 DevKit)

void setup() {
  pinMode(2, OUTPUT);  // Set GPIO2 as output
  Serial.begin(115200);  // Start serial for debugging
  Serial.println("ESP32 Blink Started");
}

void loop() {
  digitalWrite(2, HIGH);  // Turn LED on
  Serial.println("LED ON");
  delay(1000);  // Wait 1 second
  digitalWrite(2, LOW);   // Turn LED off
  Serial.println("LED OFF");
  delay(1000);  // Wait 1 second
}
