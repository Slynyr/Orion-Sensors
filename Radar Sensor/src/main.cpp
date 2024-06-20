#include <Arduino.h>
#include "radar.h"
#include "io.h"
#include "constants.h"
#include "orionFramework.h"

OrionFramework orion; 

#define PHOTO_PIN D9

void setup() {
    Serial.begin(115200);
    initializeIO();
    orion.initialize(WIFI_SSID, WIFI_PASSWORD, API_ADDRESS)
}

void loop() {
    updateLEDState();
    radarHandler();
}