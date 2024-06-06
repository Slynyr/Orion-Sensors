#include <Arduino.h>
#include <LinkedList.h>
#include "esp_camera.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Camera
#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
#include "camera_pins.h"
bool isCameraInitialized = false; 
unsigned long lastFrameTime = millis();
unsigned long frameInterval = 0; 
camera_fb_t* prevFrame = nullptr;

// Motion Detection
bool isMotionDetected = false;
const int pixelThreshold = 15; // CHANGE TO DEFINE? 
const int datesetSize = 100;
LinkedList<int> pixelSamples = LinkedList<int>();

// Serial debug
const bool WAIT_ON_SERIAL_CONNECT = true; 

int getPixelDifference(const camera_fb_t* frame1, const camera_fb_t* frame2, int threshold);
void addPixelSample(const int pixelSample);
int getPixelAverate();

void updateMotionStatus() {
  if (isCameraInitialized && (millis() - lastFrameTime) > frameInterval) {
    lastFrameTime = millis();
    camera_fb_t *frame = esp_camera_fb_get();
    
    if (frame) {
      if (prevFrame) {
        int pixelDif = getPixelDifference(prevFrame, frame, pixelThreshold);
        addPixelSample(pixelDif);
        
        // Releasing buffer from previous tick 
        delete[] prevFrame->buf; // Free the buffer
        delete prevFrame; // Free the frame structure
      }

      prevFrame = new camera_fb_t;
      prevFrame->len = frame->len;
      prevFrame->width = frame->width;
      prevFrame->height = frame->height;
      prevFrame->format = frame->format;
      prevFrame->buf = new uint8_t[frame->len];
      memcpy(prevFrame->buf, frame->buf, frame->len);

      esp_camera_fb_return(frame);
    } else {
      Serial.println("Failed to capture image");
    }
  }
}

void addPixelSample(const int pixelSample) {
  if (pixelSample != -1) {
    if (pixelSamples.size() == datesetSize) {
      pixelSamples.remove(0);
    }
    pixelSamples.add(pixelSample);
  }
}

int getPixelAverate() {
  int total = 0;

  for (int i = 0; i < pixelSamples.size(); i++){
    total += pixelSamples.get(i);
  }

  return total/pixelSamples.size();
}

int getPixelDifference(const camera_fb_t* frame1, const camera_fb_t* frame2, int threshold) {
  if (!frame1 || !frame2 || frame1->len != frame2->len) {
    return -1;
  }



  int pixelChangeCount = 0; 
  bool isFrameSame = true;

  for (size_t i = 0; i < frame1->len; i++) {
    //Serial.println(std::abs(frame1->buf[i] - frame2->buf[i]));
    if (std::abs(frame1->buf[i] - frame2->buf[i]) > threshold) {
      pixelChangeCount++;
    }

    if (frame1->buf[i] != frame2->buf[i]) {
      isFrameSame = false;
    }

    //Serial.println(isFrameSame);
  }

  return pixelChangeCount;
}


void setup() {
  // Serial 
  Serial.begin(115200);
  if (WAIT_ON_SERIAL_CONNECT) {
    while(!Serial); // Waiting on serial monitor connection for debugging
  }

  // Camera config | THIS REALLY SHOULDNT BE IN HERE
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM; // TODO Depricated objects. Update
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA;
  config.pixel_format = PIXFORMAT_RGB565; 
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;


  // Camera Init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera failed to initialize with error 0x%x", err);
    return; // Quit setup
  } else {
    isCameraInitialized = true; 
  }
 }

void loop() {
  updateMotionStatus();
}


