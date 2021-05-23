#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

uint8_t  WIFI_CHANNEL = 0;

// id of the cell to which the station and the wireless estop belong
// this allows to have multiple setups belonging to different scopes
// e.g. Wireless estop with cell id 1 will stop all master stations
// that are set to id 1, however master stations set to other ids
// will drop and ignore the message. 
uint8_t CELL_ID = 10;

// how long we wait between sending messages
unsigned long MSG_TIME_MS = 20;

// how many messages we can lose before activating E-Stop because of 
// message timeout
unsigned long SKIP_BEFORE_TIMEOUT = 5;

// Address of the central station
uint8_t BASE_STATION_MAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};

// Address of the client station (wireless estop button)
uint8_t BUTTON_STATION_MAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x02};

#endif
