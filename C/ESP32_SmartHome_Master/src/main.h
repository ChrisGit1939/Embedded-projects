#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <LittleFS.h>

extern const char *ssid;
extern const char *password;

extern IPAddress local_IP;
extern IPAddress gateway;
extern IPAddress subnet;

extern AsyncWebServer server;
extern DNSServer dns;

#define LED 2
void GPIO_config();
void BlinkTask(void *parameter);
void startBlinkTask();



String Welcome();

void wifi_transmit_init(void);
void webserver_init(void);