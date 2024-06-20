#include <Arduino.h>
#include "constants.h"
#include "radar.h"

OrionFramework orion; 

bool isLEDOn = false; 

void initializeIO() {
    pinMode(RADAR_S1, INPUT);
    pinMode(RADAR_S2, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void updateLEDState() {
    const bool currentIsLEDOn = isPresenceDetected();
    
    if (currentIsLEDOn != isLEDOn) {
        isLEDOn = currentIsLEDOn;
        digitalWrite(LED_PIN, isLEDOn ? LOW : HIGH);
    }
}