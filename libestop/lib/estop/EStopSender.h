#ifndef ESTOP_SENDER_H
#define ESTOP_SENDER_H

#include <Arduino.h>
#include "common_types.h"

namespace estop
{
    class EStopSender
    {
    public:
        EStopSender(uint8_t wifiChannel, uint8_t cellId);
        bool init();

        void sendState(bool eStopFree, float batteryVoltage);

    private:

        #ifdef ESP32
            static void messageSentCallBackStatic(const uint8_t *mac_addr, esp_now_send_status_t sendStatus);
        #else
            static void messageSentCallBackStatic(uint8_t *mac_addr, uint8_t sendStatus);
        #endif
        
        void messageSentCallBack(const uint8_t *mac_addr, uint8_t sendStatus);

        uint8_t m_wifiChannel = 0;

        estop::estop_message m_estop_message;
        static EStopSender *m_instance;
    };
}
#endif