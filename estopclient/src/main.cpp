#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Log.h>
#include <SerialLogger.h>

#define BUTTON_PIN_D2 4
#define LOOP_DELAY 20

#define WIFI_CHANNEL 0

// Address of the central station
uint8_t masterMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x01};

// Address of the client station
uint8_t clientMAC[] = {0xDE, 0xAD, 0x13, 0x37, 0x00, 0x02};


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool eStopFree;
} struct_message;

// Create a struct_message called myData
struct_message myData;


unsigned int g_messageCounter = 0;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  g_messageCounter++;
  if (sendStatus == 0){
    //Serial.println("Delivery success");
  }
  else{
    Serial.printf("Delivery failed, message number: %d\n", g_messageCounter);
  }
}

void setup() {
  Log::init(new SerialLogger());

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN_D2, INPUT);
  delay(10);

  // 2 = The chip won’t make RF calibration after waking up from Deep-sleep. Power consumption is low. 
  //system_deep_sleep_set_option(2);

  // delete old wifi settings
  WiFi.disconnect();

    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Log::info("[OLD] ESP8266 Board MAC Address: " + WiFi.macAddress());
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &clientMAC[0]);
  Log::info("[NEW] ESP8266 Board MAC Address: " + WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(masterMAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
}

void loop() {
  bool eStopFree = digitalRead(BUTTON_PIN_D2) > 0; 
  bool previousEStopFree = !eStopFree; 

  while(true){
    eStopFree = digitalRead(BUTTON_PIN_D2) > 0;
    digitalWrite(LED_BUILTIN, eStopFree);

    if(eStopFree != previousEStopFree){
      Serial.printf("Estop free changed %d -> %d\n", previousEStopFree, eStopFree);
    }
    previousEStopFree = eStopFree;
    
    strcpy(myData.a, "THIS IS A CHAR");
    myData.b = random(1,20);
    myData.c = 1.2;
    myData.eStopFree = eStopFree;

    // Send message via ESP-NOW
    esp_now_send(masterMAC, (uint8_t *) &myData, sizeof(myData));

    
    delay(LOOP_DELAY);
    //ESP.deepSleep(LOOP_DELAY);
  }  
}