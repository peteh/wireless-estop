#include <Arduino.h>
#ifndef ESTOP_RECEIVER_H
#define ESTOP_RECEIVER_H

class EStopReceiver{
    public: 
        bool isEStopFree();
        bool init();
        void addEStopClient(uint8_t *mac);
    
    private:
        void messageCallBack();

        uint m_messageCounter;
};

#endif