#ifndef COM_H
#define COM_H

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

extern WiFiClient espClient;
extern PubSubClient client;

extern StaticJsonDocument<256> doc;
extern char jbuffer[256];

enum WARPKERN
    {
    LASTFREIGABE_AUS,   
    LASTFREIGABE_EIN,
    QUELLFREIGABE_AUS,
    QUELLFREIGABE_EIN, 
    HEISZAUTOMATIK_EIN,
    HEIZAUTOMATIK_AUS,
    SELBSTHALTUNG_EIN,
    SELBSTHALTUNG_AUS
    };

void setup_wifi();
void reconnect();
void  MQTTinit();

#endif