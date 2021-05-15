#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>
#include <EStopReceiver.h>
#include "../../common/config.h"


EStopReceiver *g_estopReceiver;

void setup(){
  Log::init(new SerialLogger());

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  
  // delete old wifi settings
  WiFi.disconnect();

  Log::info("[OLD] ESP8266 Board MAC Address: " + WiFi.macAddress());
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &MASTER_MAC[0]);
  Log::info("[NEW] ESP8266 Board MAC Address: " + WiFi.macAddress());

  WiFi.mode(WIFI_STA); // Place in station mode for ESP-NOW sender node
  //disable sleep mode
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);  

  g_estopReceiver = new EStopReceiver(CLIENT_MAC, WIFI_CHANNEL, 100);
  g_estopReceiver->init();
}

bool g_estopFree = false;
bool g_timedout = true;

void loop(){

  bool eStopFree = g_estopReceiver->isEStopFree();
  bool timedout = g_estopReceiver->isTimedout();

  if(g_estopFree != eStopFree){
    Serial.printf("EStop Free changed: %d -> %d\n", g_estopFree, eStopFree);
  }

  if(g_timedout != timedout){
    Serial.printf("Timed Out: %d -> %d\n", g_timedout, timedout);
  }
  digitalWrite(LED_BUILTIN, eStopFree);
  g_estopFree = eStopFree;
  g_timedout = timedout;
  // TODO I think we can make this loop event driven and make it sleep more
}