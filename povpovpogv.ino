#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>  
#include "HT_SSD1306Wire.h"

static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int x,y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int px, py;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X: ");
  Serial.println(myData.x);
  Serial.print("Y: ");
  Serial.println(myData.y);
  delay(100);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  display.init();
  display.clear();
  display.display();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {
  px = map(myData.x, 0, 4096, 0, display.getWidth()-1);
  py = map(myData.y, 0, 4096, 0, display.getHeight()-1);
  display.drawCircle(px, py, 5);
  display.display();
  display.clear();
  delay(100);

}