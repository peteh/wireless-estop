#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

uint8_t  WIFI_CHANNEL = 0;

// id of the cell to which the station and the wireless estop belong
// this allows to have multiple setups belonging to different scopes
// e.g. Wireless estop with cell id 1 will stop all master stations
// that are set to id 1, however master stations set to other ids
// will drop and ignore the message. 
uint8_t CELL_ID = 10;

// Address of the central station
uint8_t MASTER_MAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};

// Address of the client station (wireless estop button)
uint8_t CLIENT_MAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x02};

#endif
