#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>

// Address of the central station
uint8_t masterMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};

// Address of the client station
uint8_t clientMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x02};

int g_messageCounter = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {

  if(g_messageCounter % 1 == 3)
  {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Char: ");
    Serial.println(myData.a);
    Serial.print("Int: ");
    Serial.println(myData.b);
    Serial.print("Float: ");
    Serial.println(myData.c);
    Serial.print("String: ");
    Serial.println(myData.d);
    Serial.print("Bool: ");
    Serial.println(myData.e);
    Serial.println();
  }
  g_messageCounter++;
}

void setup(){
  Log::init(new SerialLogger());

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  
  Log::info("[OLD] ESP8266 Board MAC Address: " + WiFi.macAddress());
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &masterMAC[0]);
  Log::info("[NEW] ESP8266 Board MAC Address: " + WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Log::error("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);

}
 
void loop(){
  digitalWrite(LED_BUILTIN, g_messageCounter % 2);
}