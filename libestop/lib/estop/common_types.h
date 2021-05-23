#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

// Structure to receive data
// Must match the sender structure
typedef struct estop_message {
    char header[5]; // ESTOP
    uint8_t messageNum;
    uint8_t cellId; 
    bool eStopFree;
    float batteryVoltage;
} estop_message;

#endif
