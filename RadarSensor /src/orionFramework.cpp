#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "constants.h"

bool isConnected = false; 

void initializeOrionFramework() {

    Serial.begin(115200);
    Serial.println("Connecting to WiFi network");

    // Connecting to WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
  
    isConnected = true; 
    Serial.println("Connected to WiFi network");
}

bool getWiFiState() {
    return isConnected;
}

void sendTrigger(String triggerName, String triggerType) {
    int attemptCount = 0;

    if (WiFi.status() == WL_CONNECTED) {
        String targetAddress = String("http://") + API_ADDRESS + "/" + triggerName + "/" + triggerType;
        
        while (attemptCount < MAX_ATTEMPTS){
            attemptCount++;
            
            HTTPClient http; 
            http.begin(targetAddress);
            int httpResponse = http.GET();
            
            if (httpResponse > 0) {
                Serial.println("[REQUEST] Request sent successfully"); // TODO handle codes
                http.end();
                break; 
            } else {
                Serial.println("[REQUEST] Something went wrong while attempting to post request");
            }

            http.end();
        }
    } else {
        Serial.println("WiFi unavailable. Unable to send request");
    }
}