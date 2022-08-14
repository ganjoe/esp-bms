#ifndef COM_H
#define COM_H

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

extern WiFiClient espClient;
extern PubSubClient client;

extern StaticJsonDocument<256> doc;
extern char jbuffer[256];

#endif