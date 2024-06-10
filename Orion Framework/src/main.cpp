#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

char* wifiSSID = "MYHDSB";
char* wifiPassword = "";
char* apiAddress = "localhost";
char* triggerName = "downstairs";

void initializeOrionFramework();

void setup() {
  initializeOrionFramework();
}

void loop() {

}

void initializeOrionFramework() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
}

//Setter methods
void setSSID(char ssid) {
  wifiSSID = ssid;
}

void setPassword(char password) {
  wifiPassword = password; 
}

void setAPIAddress(char address) {
  apiAddress = address; 
}

void setTrigger(char trigger) {
  triggerName = trigger;
}

// API Methods
void sendTrigger() {
  if (WiFi.status() == WL_CONNECTED) {
    String targetAddress = String("http://") + apiAddress + "/" + triggerName;
    
    HTTPClient http; 
    http.begin(targetAddress);
    int httpResponse = http.GET();
    if (httpResponse > 0) {
      Serial.println("Request sent successfully"); // TODO handle codes
    } else {
      Serial.println("Something went wrong when sending request");
    }

    http.end();

  } else {
    Serial.println("WiFi unavailable. Unable to send request");
  }
}