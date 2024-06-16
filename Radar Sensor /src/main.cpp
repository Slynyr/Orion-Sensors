#include <Arduino.h>
#include "radar.h"
#include "io.h"
#include "orionFramework.h"

#define PHOTO_PIN D9

void setup() {
    Serial.begin(115200);
    initializeIO();
    initializeOrionFramework();
}

void loop() {
    updateLEDState();
    radarHandler();
}