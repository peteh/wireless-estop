#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>

#include "../../common/config.h"
#include <common_types.h>
#include <EStopSender.h>

#define BUTTON_PIN_D2 D2
#define ANALOG_PIN_A0 A0

bool g_previousEStopFree = false;

estop::EStopSender* g_eStopSender;

void setup()
{
  Log::init(new SerialLogger());

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN_D2, INPUT);
  pinMode(ANALOG_PIN_A0, INPUT);

  delay(10);

    // delete old wifi settings
  WiFi.disconnect();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Log::infof("[OLD] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &BUTTON_STATION_MAC[0]);
  Log::infof("[NEW] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());

  g_eStopSender = new estop::EStopSender(WIFI_CHANNEL, CELL_ID);

  // init communication
  g_eStopSender->init();

  // Register peer
  // TODO: we only use broadcast, no need to add base station as as peer
  //esp_now_add_peer(BASE_STATION_MAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
  
  // init vars to force update
  g_previousEStopFree = !(digitalRead(BUTTON_PIN_D2) > 0);
}

void loop()
{
  bool eStopFree = digitalRead(BUTTON_PIN_D2) > 0;
  float raw = analogRead(A0);
  float voltage = raw / 1023.0 * 4.2;

  digitalWrite(LED_BUILTIN, eStopFree);

  if (eStopFree != g_previousEStopFree)
  {
    Log::infof("Estop free changed %d -> %d", g_previousEStopFree, eStopFree);
    Log::infof("Voltage: %f", voltage);
  }

  // Send message via ESP-NOW
  g_eStopSender->sendState(eStopFree, voltage);
  g_previousEStopFree = eStopFree;

  delay(MSG_TIME_MS);
}