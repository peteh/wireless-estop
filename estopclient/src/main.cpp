#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>

#include "../../common/config.h"
#include <common_types.h>

#define BUTTON_PIN_D2 D2
#define LOOP_DELAY 20
#define ANALOG_PIN_A0 A0

// Create a struct_message called g_sendEstopMessage
estop_message g_sendEstopMessage;

unsigned int g_messageCounter = 0;
bool g_eStopFree = false; 
bool g_previousEStopFree = !g_eStopFree; 
float g_voltage;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  g_messageCounter++;
  if (sendStatus != 0){
    //Log::errorf("Delivery failed, message number: %d, Error Code: %d", g_messageCounter, sendStatus);
  }
}

void setup() {
  Log::init(new SerialLogger());

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN_D2, INPUT);
  pinMode(ANALOG_PIN_A0, INPUT);

  delay(10);

  // 2 = The chip won’t make RF calibration after waking up from Deep-sleep. Power consumption is low. 
  //system_deep_sleep_set_option(2);

  // delete old wifi settings
  WiFi.disconnect();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Log::infof("[OLD] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &CLIENT_MAC[0]);
  Log::infof("[NEW] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(MASTER_MAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);

  // init vars to force update
  g_eStopFree = digitalRead(BUTTON_PIN_D2) > 0; 
  g_previousEStopFree = !g_eStopFree; 
}

void loop() {
  g_eStopFree = digitalRead(BUTTON_PIN_D2) > 0;

float raw = analogRead(A0);
  g_voltage =raw/1023.0 * 4.2;

  digitalWrite(LED_BUILTIN, g_eStopFree);

  if(g_eStopFree != g_previousEStopFree){
    Log::infof("Estop free changed %d -> %d", g_previousEStopFree, g_eStopFree);
    Log::infof("Voltage: %f", g_voltage);
  }

  

  
  g_sendEstopMessage.eStopFree = g_eStopFree;
  g_sendEstopMessage.messageNum = g_messageCounter;
  g_sendEstopMessage.batteryVoltage = g_voltage;
  // Send message via ESP-NOW
  esp_now_send(MASTER_MAC, (uint8_t *) &g_sendEstopMessage, sizeof(g_sendEstopMessage));

  
  g_previousEStopFree = g_eStopFree;

  delay(LOOP_DELAY);
  //ESP.deepSleep(LOOP_DELAY);
}