#ifndef ESTOP_RECEIVER_H
#define ESTOP_RECEIVER_H

#include <Arduino.h>
#include "common_types.h"
#include "EStop.h"

namespace estop
{
    class EStopReceiver
    {
    public:
        
        EStopReceiver(const uint8_t *clientMac, uint8_t wifiChannel, uint8_t cellId, unsigned long timeoutMs);
        bool init();
        bool isEStopFree();
        bool isTimedout();

        EStopState getEStopState();
        float getBatteryVoltage();

    private:
        
        #ifdef ESP32
            static void messageCallBackStatic(const unsigned char *mac, const unsigned char *incomingData, int len);
        #else
            static void messageCallBackStatic(uint8_t *mac, uint8_t *incomingData, uint8_t len);
        #endif

        void messageCallBack(const uint8_t *mac, const uint8_t *incomingData, uint8_t len);

        uint8_t m_clientMAC[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint m_messageCounter = 0;
        uint8_t m_wifiChannel = 0;
        uint8_t m_cellId = 0;

        estop_message m_estop_message;
        unsigned long m_lastMessageTimestamp;
        unsigned long m_timeoutMs;
        static EStopReceiver *m_instance;
    };
}
#endif