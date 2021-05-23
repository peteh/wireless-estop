#ifndef ESTOP_SENDER_H
#define ESTOP_SENDER_H

#include <Arduino.h>
#include "common_types.h"

class EStopSender{
    public: 
        EStopSender(uint8_t wifiChannel, uint8_t cellId);
        bool init();

        void sendState(bool eStopFree, float batteryVoltage);

    private:
        static void messageSentCallBackStatic(uint8_t *mac_addr, uint8_t sendStatus);
        void messageSentCallBack(uint8_t *mac_addr, uint8_t sendStatus);

        uint8_t m_wifiChannel = 0;

        estop_message m_estop_message;
        static EStopSender *m_instance;
};

#endif