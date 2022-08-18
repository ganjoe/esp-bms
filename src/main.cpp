#include <Arduino.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Battery_Cells.h"
#include "credentials.h"
#include "com.h"

/* Ausgänge für Relais */

#define SELBSTHALTUNG D5
#define BATTPROTECT_LOAD D6   //BP0
#define BATTPROTECT_CHARGE D7
#define DISCONNECT D3

void setup() 
     {
     Serial.begin(115200); 
     MQTTinit();
     setup_wifi();
     scan_i2c();  

     pinMode(BATTPROTECT_LOAD, OUTPUT);
     pinMode(BATTPROTECT_CHARGE, OUTPUT);
     pinMode(SELBSTHALTUNG, OUTPUT);
     pinMode(DISCONNECT,INPUT_PULLUP);
     Serial.print("setup done");
     }


void loop() 
{
   StaticJsonDocument<256> doc; // ArduinoJson6 Static Json Buffer
  char jbuffer[256];
  
  if (!client.connected())   {    reconnect();  }  client.loop();

    /* topic warpkern senden */
    doc["cell01"] = Batt.getCellVoltage(0);
    doc["cell02"] = Batt.getCellVoltage(1);
    doc["cell03"] = Batt.getCellVoltage(2);
    doc["cell04"] = Batt.getCellVoltage(3);
    doc["cell05"] = Batt.getCellVoltage(4);
    doc["cell06"] = Batt.getCellVoltage(5);
    doc["cell07"] = Batt.getCellVoltage(6);
    doc["cell08"] = Batt.getCellVoltage(7);
    doc["tempboden"] = Batt.Heater->getTemperature();

    size_t bytes = serializeJson(doc, jbuffer);
    client.publish("warpkern_daten", jbuffer, bytes);


    /* GPIOS setzen */
     digitalWrite(BATTPROTECT_LOAD, Batt.StateDischarge);
     digitalWrite(BATTPROTECT_CHARGE, Batt.StateCharge);
     digitalWrite(SELBSTHALTUNG, Batt.StateESP);

     Batt.update();

     /* GPIOS lesen */
     if (!digitalRead(DISCONNECT)) //invertiert
     {
          Serial.println("disconnect");
     }
}



