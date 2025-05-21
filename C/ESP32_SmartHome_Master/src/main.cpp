#include "main.h"




void setup() 
{
  Serial.begin(9600);
  Serial.println("Start Device");
  GPIO_config();

  wifi_transmit_init();

  startBlinkTask();

  

  if (!LittleFS.begin()) {
    Serial.println("Błąd montowania LittleFS");
    return;
  }
  




}

void loop() 
{
  // Uruchamianie serwera DNS, aby przekierowywał na ESP32
  dns.processNextRequest();
}


