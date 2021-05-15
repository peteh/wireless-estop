#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

// Structure example to receive data
// Must match the sender structure
typedef struct estop_message {
    //char a[32];
    //int b;
    //float c;
    unsigned int messageNum;
    bool eStopFree;
} estop_message;

#endif
