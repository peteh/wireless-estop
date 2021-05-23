#ifdef ESP32
  #include <WiFi.h>
  #include <esp_now.h>
#else
  #include <ESP8266WiFi.h>
  #include <espnow.h>
#endif

#include "EStopSender.h"
#include <Log.h>

namespace estop
{
    // disgusting hack to solve the callback only
    // be possible to static functions
    EStopSender *EStopSender::m_instance = NULL;

    EStopSender::EStopSender(uint8_t wifiChannel, uint8_t cellId)
        : m_wifiChannel(wifiChannel)
    {
        m_estop_message.messageNum = 0;
        m_estop_message.eStopFree = false;
        m_estop_message.cellId = cellId;
        m_estop_message.head[0] = 'E';
        m_estop_message.head[1] = 'S';
        m_estop_message.head[2] = 'T';
        m_estop_message.head[3] = 'O';
        m_estop_message.head[4] = 'P';
    }

    bool EStopSender::init()
    {
        if (m_instance != NULL)
        {
            Log::error("EStopSender initialized multiple times but should only be done once");
            return false;
        }
        EStopSender::m_instance = this;

        // Init ESP-NOW
        if (esp_now_init() != 0)
        {
            Log::error("Error initializing ESP-NOW");
            return false;
        }

        // Once ESPNow is successfully Init, we will register for recv CB to
        // get recv packer info
        
        

        #ifdef ESP32
            // nothing
        #else
            esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
        #endif

        esp_now_register_send_cb(EStopSender::messageSentCallBackStatic);

        return true;
    }

    #ifdef ESP32
        // HACK: ugly hack to call our member function because espnow does not allow
        // registering non static functions
        void EStopSender::messageSentCallBackStatic(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
        {
            // TODO: fix send status
            m_instance->messageSentCallBack(mac_addr, (uint8_t) 0);
        }

    #else
        // HACK: ugly hack to call our member function because espnow does not allow
        // registering non static functions
        void EStopSender::messageSentCallBackStatic(uint8_t *mac_addr, uint8_t sendStatus)
        {
            m_instance->messageSentCallBack(mac_addr, sendStatus);
        }
    #endif
    

    // Callback function that will be executed when data is received
    void EStopSender::messageSentCallBack(const uint8_t *mac_addr, uint8_t sendStatus)
    {
        m_estop_message.messageNum++;
    }

    void EStopSender::sendState(bool eStopFree, float batteryVoltage)
    {
        m_estop_message.batteryVoltage = batteryVoltage;
        m_estop_message.eStopFree = eStopFree;

        // Broadcast address
        uint8_t broadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        esp_now_send(broadcastMac, (uint8_t *)&m_estop_message, sizeof(m_estop_message));
    }
}