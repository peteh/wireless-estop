#include <Arduino.h>
#define BUTTON_PIN_D2 4
#define LOOP_DELAY 20

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN_D2, INPUT);
  delay(10);
  Serial.flush();
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
    Serial.flush();
    delay(LOOP_DELAY);
  }  
}