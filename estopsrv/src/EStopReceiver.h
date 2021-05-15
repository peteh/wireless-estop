#ifndef ESTOP_RECEIVER_H
#define ESTOP_RECEIVER_H

#include <Arduino.h>
#include "../../common/common_types.h"





class EStopReceiver{
    public: 
        enum EStopState { ESTOP_FREE, ESTOP_ACTIVE, ESTOP_TIMEOUT };
        
        EStopReceiver(const uint8_t *clientMac, uint8_t wifiChannel, unsigned long timeoutMs);
        bool init();
        bool isEStopFree();
        bool isTimedout();

        EStopState getEStopState();

    private:
        static void messageCallBackStatic(uint8_t * mac, uint8_t *incomingData, uint8_t len);
        void messageCallBack(uint8_t * mac, uint8_t *incomingData, uint8_t len);

        uint8_t m_clientMAC[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        uint m_messageCounter = 0;
        uint8_t m_wifiChannel = 0;

        estop_message m_estop_message;
        unsigned long m_lastMessageTimestamp;
        unsigned long m_timeoutMs;
        static EStopReceiver *m_instance;
};

#endif