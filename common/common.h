#define WIFI_CHANNEL 0

// Address of the central station
uint8_t masterMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};

// Address of the client station
uint8_t clientMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x02};

// Structure example to receive data
// Must match the sender structure
typedef struct estop_message {
    //char a[32];
    //int b;
    //float c;
    unsigned int messageNum;
    bool eStopFree;
} estop_message;
