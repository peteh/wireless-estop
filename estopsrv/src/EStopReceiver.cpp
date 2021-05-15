#include <ESP8266WiFi.h>
#include <espnow.h>
#include "EStopReceiver.h"
#include <Log.h>

// disgusting hack to solve the callback only
// be possible to static functions
EStopReceiver *EStopReceiver::m_instance = NULL;

EStopReceiver::EStopReceiver(const uint8_t *clientMac, uint8_t wifiChannel, unsigned long timeoutMs)
    : m_wifiChannel(wifiChannel),
      m_lastMessageTimestamp(0),
      m_timeoutMs(timeoutMs)
{
    memcpy(&m_clientMAC[0], clientMac, sizeof(m_clientMAC));
    // TODO initialize estopmessage struct
}

bool EStopReceiver::init()
{
    if (m_instance != NULL)
    {
        Log::error("EStopReceiver initialized multiple times but should only be done once");
        return false;
    }
    EStopReceiver::m_instance = this;

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Log::error("Error initializing ESP-NOW");
        return false;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(EStopReceiver::messageCallBackStatic);
    esp_now_add_peer(m_clientMAC, ESP_NOW_ROLE_CONTROLLER, m_wifiChannel, NULL, 0);

    return true;
}

// HACK: ugly hack to call our member function because espnow does not allow
// registering non static functions
void EStopReceiver::messageCallBackStatic(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    m_instance->messageCallBack(mac, incomingData, len);
}

// Callback function that will be executed when data is received
void EStopReceiver::messageCallBack(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    if (memcmp(mac, m_clientMAC, sizeof(m_clientMAC)) != 0)
    {
        // TODO: add mac to error message
        Log::error("Received unexpected message from xx");
        return;
    }

    if (len != sizeof(estop_message))
    {
        // TODO: add mac to error message
        Log::error("Received malformed message from xx");
        return;
    }

    estop_message *incoming = (estop_message *)incomingData;

    if (m_estop_message.eStopFree != incoming->eStopFree)
    {
        //Serial.printf("Estop free changed %d -> %d\n", m_estop_message.eStopFree, incoming->eStopFree);
    }

    memcpy(&m_estop_message, incomingData, sizeof(m_estop_message));
    m_lastMessageTimestamp = millis();
    if (false)
    {
        Serial.print("Bytes received: ");
        Serial.println(len);
        Serial.println(m_estop_message.eStopFree);
        Serial.printf("> Successfully received Local MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
                      (unsigned char)mac[0],
                      (unsigned char)mac[1],
                      (unsigned char)mac[2],
                      (unsigned char)mac[3],
                      (unsigned char)mac[4],
                      (unsigned char)mac[5]);
        Serial.println();
    }
    m_messageCounter++;
}

bool EStopReceiver::isEStopFree()
{
    if(isTimedout()){
        return false;
    }
    return m_estop_message.eStopFree;
}

bool EStopReceiver::isTimedout(){
    return millis() > m_lastMessageTimestamp + m_timeoutMs;
}