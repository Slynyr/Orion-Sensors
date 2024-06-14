#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "orionFramework.h"

void setup() {
  initializeOrionFramework();
}

void loop() {
  //Serial.println(getWiFiState());
  sendTrigger("button1", "ON");
  delay(5000);
}