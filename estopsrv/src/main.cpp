#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>

#include "../../common/common.h"


int g_messageCounter = 0;

// Create a struct_message called myData
estop_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  if(memcmp(mac, clientMAC, sizeof(clientMAC)) != 0)
  {
    // TODO: add mac to error message
    Log::error("Received unexpected message from xx");
    return;
  }

  if(len != sizeof(estop_message)){
    // TODO: add mac to error message
    Log::error("Received malformed message from xx");
    return;
  }

  estop_message* incoming = (estop_message*) incomingData;

  if(myData.eStopFree != incoming->eStopFree)
  {
    Serial.printf("Estop free changed %d -> %d\n", myData.eStopFree, incoming->eStopFree);
  }

  memcpy(&myData, incomingData, sizeof(myData));

  if(false)
  {
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.println(myData.eStopFree);
    Serial.printf("> Successfully received Local MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
    (unsigned char) mac[0],
    (unsigned char) mac[1],
    (unsigned char) mac[2],
    (unsigned char) mac[3],
    (unsigned char) mac[4],
    (unsigned char) mac[5]);
    Serial.println();
  }
  g_messageCounter++;
}

void setup(){
  Log::init(new SerialLogger());

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  
  // delete old wifi settings
  WiFi.disconnect();

  Log::info("[OLD] ESP8266 Board MAC Address: " + WiFi.macAddress());
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &masterMAC[0]);
  Log::info("[NEW] ESP8266 Board MAC Address: " + WiFi.macAddress());

  WiFi.mode(WIFI_STA); // Place in station mode for ESP-NOW sender node
  //disable sleep mode
  WiFi.setSleepMode(WIFI_NONE_SLEEP);  

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Log::error("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(clientMAC, ESP_NOW_ROLE_CONTROLLER, WIFI_CHANNEL, NULL, 0);
}
 
void loop(){
  digitalWrite(LED_BUILTIN, myData.eStopFree);
}