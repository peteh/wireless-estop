#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>
#include <EStopReceiver.h>
#include "../../common/config.h"

#define PIN_RELAY D2

estop::EStopReceiver *g_estopReceiver;

void setup()
{
  Log::init(new SerialLogger());

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);


  // delete old wifi settings
  WiFi.disconnect();

  Log::infof("[OLD] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &BASE_STATION_MAC[0]);
  Log::infof("[NEW] ESP8266 Board MAC Address: %s", WiFi.macAddress().c_str());

  WiFi.mode(WIFI_STA); // Place in station mode for ESP-NOW sender node
  //disable sleep mode
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  g_estopReceiver = new estop::EStopReceiver(BUTTON_STATION_MAC, WIFI_CHANNEL, CELL_ID, MSG_TIME_MS * SKIP_BEFORE_TIMEOUT);
  g_estopReceiver->init();
}

estop::EStopState g_eStopState = estop::ESTOP_ACTIVE;

String eStopStateToStr(estop::EStopState eStopState)
{
  switch (eStopState)
  {
  case estop::ESTOP_ACTIVE:
    return "ESTOP_ACTIVE";
  case estop::ESTOP_FREE:
    return "ESTOP_FREE";
  case estop::ESTOP_TIMEOUT:
    return "ESTOP_TIMEOUT";
  default:
    return "UNDEFINED_TYPE";
  }
}
void loop()
{

  estop::EStopState eStopState = g_estopReceiver->getEStopState();

  if (g_eStopState != eStopState)
  {
    Log::infof("EStop State changed: %s -> %s (Battery: %fV)",
               eStopStateToStr(g_eStopState).c_str(), 
               eStopStateToStr(eStopState).c_str(), 
               g_estopReceiver->getBatteryVoltage());
  }

  digitalWrite(LED_BUILTIN, eStopState == estop::ESTOP_FREE);
  digitalWrite(PIN_RELAY, eStopState == estop::ESTOP_FREE);
  g_eStopState = eStopState;
  // TODO I think we can make this loop event driven and make it sleep more
}