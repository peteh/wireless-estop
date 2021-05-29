#ifdef ESP32
  #include <WiFi.h>
  #include <esp_now.h>
#else
  #include <ESP8266WiFi.h>
  #include <espnow.h>
#endif

#include "EStopReceiver.h"
#include <Log.h>

namespace estop
{
    // disgusting hack to solve the callback only
    // be possible to static functions
    EStopReceiver *EStopReceiver::m_instance = NULL;

    EStopReceiver::EStopReceiver(const uint8_t *clientMac, uint8_t wifiChannel, const uint8_t cellId, unsigned long timeoutMs)
        : m_wifiChannel(wifiChannel),
          m_cellId(cellId),
          m_lastMessageTimestamp(0),
          m_timeoutMs(timeoutMs)
    {
        memcpy(&m_clientMAC[0], clientMac, sizeof(m_clientMAC));
        m_estop_message.head[0] = 'E';
        m_estop_message.head[1] = 'S';
        m_estop_message.head[2] = 'T';
        m_estop_message.head[3] = 'O';
        m_estop_message.head[4] = 'P';
        m_estop_message.messageNum = 0;
        m_estop_message.eStopFree = false;
        m_estop_message.batteryVoltage = 0.;
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
        // get recv packet info

        
        #ifdef ESP32
            // nothing
        #else
            esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
        #endif

        esp_now_register_recv_cb(EStopReceiver::messageCallBackStatic);

        #ifdef ESP32
            esp_now_peer_info_t peer;
            memcpy(peer.peer_addr, m_clientMAC, 6);
            peer.ifidx = ESP_IF_WIFI_STA;
            peer.channel = m_wifiChannel;
            peer.encrypt = false;
            esp_err_t code = esp_now_add_peer(&peer);

            switch(code)
            {
                case ESP_OK: 
                    // nothing to do
                    break;
                case ESP_ERR_ESPNOW_NOT_INIT: 
                    Log::info("ESP_ERR_ESPNOW_NOT_INIT");
                    break;
                case ESP_ERR_ESPNOW_ARG: 
                    Log::info("ESP_ERR_ESPNOW_ARG");
                    break;
                case ESP_ERR_ESPNOW_FULL: 
                    Log::info("ESP_ERR_ESPNOW_FULL");
                    break;
                case ESP_ERR_ESPNOW_NO_MEM: 
                    Log::info("ESP_ERR_ESPNOW_NO_MEM");
                    break;
                case ESP_ERR_ESPNOW_EXIST: 
                    Log::info("ESP_ERR_ESPNOW_EXIST");
                    break;
                default:
                 Log::error("Unknown");
                 break;
            }
            if ( code != ESP_OK) {
                Log::errorf("Failed adding peer with code: %d", code);
            }  
        #else
            esp_now_add_peer(m_clientMAC, ESP_NOW_ROLE_CONTROLLER, m_wifiChannel, NULL, 0);
        #endif
        

        return true;
    }

    // HACK: ugly hack to call our member function because espnow does not allow
    // registering non static functions
    #ifdef ESP32
        void EStopReceiver::messageCallBackStatic(const unsigned char *mac, const unsigned char *incomingData, int len)
        {
            m_instance->messageCallBack(mac, incomingData, len);
        }

    #else
        void EStopReceiver::messageCallBackStatic(uint8_t *mac, uint8_t *incomingData, uint8_t len)
        {
            m_instance->messageCallBack(mac, incomingData, len);
        }
    #endif

    // Callback function that will be executed when data is received
    void EStopReceiver::messageCallBack(const uint8_t *mac, const uint8_t *incomingData, uint8_t len)
    {

        if (memcmp(mac, m_clientMAC, sizeof(m_clientMAC)) != 0)
        {
            Log::errorf("Received unexpected message from from unexpected MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                       (unsigned char)mac[0],
                       (unsigned char)mac[1],
                       (unsigned char)mac[2],
                       (unsigned char)mac[3],
                       (unsigned char)mac[4],
                       (unsigned char)mac[5]);
            return;
        }

        if (len != sizeof(estop_message))
        {
            Log::errorf("Received wrong length message from MAC: %02x:%02x:%02x:%02x:%02x:%02x (expected: %d, is: %d)",
                       (unsigned char)mac[0],
                       (unsigned char)mac[1],
                       (unsigned char)mac[2],
                       (unsigned char)mac[3],
                       (unsigned char)mac[4],
                       (unsigned char)mac[5],
                       sizeof(estop_message),
                       len);
            return;
        }

        estop_message *incoming = (estop_message *)incomingData;
        char estopHeader[] = "ESTOP";

        if (memcmp(incoming, &estopHeader[0], 5) != 0)
        {
            Log::errorf("Header mismatch, expected: 'ESTOP' from MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                       (unsigned char)mac[0],
                       (unsigned char)mac[1],
                       (unsigned char)mac[2],
                       (unsigned char)mac[3],
                       (unsigned char)mac[4],
                       (unsigned char)mac[5]);
            return;
        }

        if (incoming->cellId != m_cellId)
        {
            Log::errorf("Cell id mismatch, ignoring message (expected: %d, is: %d)", m_cellId, incoming->cellId);
            return;
        }

        if (m_estop_message.eStopFree != incoming->eStopFree)
        {
            // TODO add callback function, but update state first and then calculate full state
            // this place here will not perfectly work as we wont handle timeout
        }
        memcpy(&m_estop_message, incomingData, sizeof(m_estop_message));
        m_lastMessageTimestamp = millis();
        m_messageCounter++;
    }

    bool EStopReceiver::isEStopFree()
    {
        if (isTimedout())
        {
            return false;
        }
        return m_estop_message.eStopFree;
    }

    EStopState EStopReceiver::getEStopState()
    {
        if (isTimedout())
        {
            return ESTOP_TIMEOUT;
        }
        if (isEStopFree())
        {
            return ESTOP_FREE;
        }
        return ESTOP_ACTIVE;
    }

    float EStopReceiver::getBatteryVoltage()
    {
        return m_estop_message.batteryVoltage;
    }

    bool EStopReceiver::isTimedout()
    {
        return millis() > m_lastMessageTimestamp + m_timeoutMs;
    }
}