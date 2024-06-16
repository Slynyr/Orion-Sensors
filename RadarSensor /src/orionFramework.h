#include <Arduino.h>
#ifndef ORIONFRAMEWORK_H
#define ORIONFRAMEWORK_H

void initializeOrionFramework();

bool getWiFiState();

void sendTrigger(String triggerName, String triggerType);

#endif