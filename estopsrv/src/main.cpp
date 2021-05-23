#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>
#include <EStopReceiver.h>
#include "../../common/config.h"

EStopReceiver *g_estopReceiver;

void setup()
{
  Log::init(new SerialLogger());

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);

  // delete old wifi settings
  WiFi.disconnect();

  Log::infof("[OLD] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &MASTER_MAC[0]);
  Log::infof("[NEW] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());

  WiFi.mode(WIFI_STA); // Place in station mode for ESP-NOW sender node
  //disable sleep mode
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  g_estopReceiver = new EStopReceiver(CLIENT_MAC, WIFI_CHANNEL, CELL_ID, 100);
  g_estopReceiver->init();
}

EStopReceiver::EStopState g_eStopState = EStopReceiver::ESTOP_ACTIVE;

String eStopStateToStr(EStopReceiver::EStopState eStopState)
{
  switch (eStopState)
  {
  case EStopReceiver::ESTOP_ACTIVE:
    return "ESTOP_ACTIVE";
  case EStopReceiver::ESTOP_FREE:
    return "ESTOP_FREE";
  case EStopReceiver::ESTOP_TIMEOUT:
    return "ESTOP_TIMEOUT";
  default:
    return "UNDEFINED_TYPE";
  }
}
void loop()
{

  EStopReceiver::EStopState eStopState = g_estopReceiver->getEStopState();

  if (g_eStopState != eStopState)
  {
    Log::infof("EStop State changed: %s -> %s (Battery: %fV)",
               eStopStateToStr(g_eStopState).c_str(), 
               eStopStateToStr(eStopState).c_str(), 
               g_estopReceiver->getBatteryVoltage());
  }

  digitalWrite(LED_BUILTIN, eStopState == EStopReceiver::ESTOP_FREE);
  g_eStopState = eStopState;
  // TODO I think we can make this loop event driven and make it sleep more
}