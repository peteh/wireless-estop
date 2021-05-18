#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

// Structure to receive data
// Must match the sender structure
typedef struct estop_message {
    //char a[32];
    //int b;
    
    unsigned int messageNum;
    bool eStopFree;
    float batteryVoltage;
} estop_message;


#endif
