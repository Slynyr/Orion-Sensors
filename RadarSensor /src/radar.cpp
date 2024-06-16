#include <Arduino.h>
#include "constants.h"
#include "orionFramework.h"

bool prevPresenceState = false; 
long lastTimeOn = millis();

bool isPresenceDetected() {
    return digitalRead(RADAR_S1);
}

void radarHandler() {
    bool presenceState = isPresenceDetected();

    if (presenceState) {
        lastTimeOn = millis();
    }

    if (prevPresenceState != presenceState) {
        prevPresenceState = presenceState;

        if (prevPresenceState) {
            sendTrigger("/group/Office", "on");
        }
    } else if ((millis() - lastTimeOn) > LIGHT_TIMEOUT_DELAY && !presenceState && prevPresenceState) {
        prevPresenceState = presenceState;

        sendTrigger("/group/Office", "off");
    }
}