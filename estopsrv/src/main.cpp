#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SerialLogger.h>

// Set master mac address
uint8_t newMACAddress[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};
Connectivity
void setup(){
  Log::init(new SerialLogger());

  Serial.begin(115200);

  Serial.println();
  
  WiFi.mode(WIFI_STA);
  
  Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // ESP32 Board add-on before version < 1.0.5
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &newMACAddress[0]);
  
  // ESP32 Board add-on after version > 1.0.5
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}