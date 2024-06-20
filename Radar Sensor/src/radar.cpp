#include <Arduino.h>
#include "constants.h"
#include "orionFramework.h"

bool prevPresenceState = false; 
bool isOffTriggerSent = false;
long lastTimeOn = millis();

bool isPresenceDetected() {
    return digitalRead(RADAR_S1);
}

void radarHandler() {
    bool presenceState = isPresenceDetected();

    // If presence is detected, update the last time presence was detected
    if (presenceState) {
        lastTimeOn = millis();
        isOffTriggerSent = false; 
    }

    // Check if the presence state has changed
    if (prevPresenceState != presenceState) {
        prevPresenceState = presenceState;

        if (presenceState) {
            orion.sendTrigger("group/Office/on");
        }
    }

    // Check if the light should be turned off due to timeout
    if (!presenceState && (millis() - lastTimeOn) > LIGHT_TIMEOUT_DELAY && !isOffTriggerSent) {
        orion.sendTrigger("group/Office/off);
        isOffTriggerSent = true;
    }
}