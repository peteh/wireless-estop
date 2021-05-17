#ifndef ESTOP_SENDER_H
#define ESTOP_SENDER_H

#include <Arduino.h>
#include "common_types.h"

class EStopSender{
    public: 

        EStopSender(const uint8_t *masterMac, uint8_t wifiChannel, unsigned long timeoutMs);
        bool init();

    private:
        static void messageSentCallBackStatic(uint8_t * mac, uint8_t *incomingData, uint8_t len);
        void messageSentCallBack(uint8_t * mac, uint8_t *incomingData, uint8_t len);

        uint8_t m_clientMAC[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint m_messageCounter = 0;
        uint8_t m_wifiChannel = 0;

        estop_message m_estop_message;
        unsigned long m_lastMessageTimestamp;
        unsigned long m_timeoutMs;
        static EStopSender *m_instance;
};

#endif