#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Battery_Cells.h"
#include "com.h"

/* Treiber-Klassen für beide ADS1115 initalisieren */
ADS1115 ADS0(0x48);      //addr -> vcc
ADS1115 ADS1(0x49);      //addr -> gnd




/* User-Klassen für jeden Messkanal initalisieren */
classCell cell01(0, 1, &ADS0);
classCell cell02(1, 2, &ADS0);
classCell cell03(2, 3, &ADS0);
classCell cell04(3, 4, &ADS0);

classCell cell05(0, 5, &ADS1);
classCell cell06(1, 6, &ADS1);
classCell cell07(2, 7, &ADS1);
classCell cell08(3, 8, &ADS1);


classBattery Batt;


void setup() 
{
WiFi.mode(WIFI_STA);
WiFi.begin("Level5", "Hallogaga#1");
while (WiFi.status() != WL_CONNECTED)   {    delay(500);   }
client.setServer("10.20.0.34", 1883);
client.connect("Battery");
client.setCallback(Battery_callback);
client.subscribe("warpkern_last");

Serial.begin(115200); 

scan_i2c();  


/* Batterie-Klasse mit cell-objekt-referenzen füllen */

Batt.cell[0] = &cell01;
Batt.cell[1] = &cell02;
Batt.cell[2] = &cell03;
Batt.cell[3] = &cell04;
Batt.cell[4] = &cell05;
Batt.cell[5] = &cell06;
Batt.cell[6] = &cell07;
Batt.cell[7] = &cell08;

ADS0.begin();
ADS1.begin();

Serial.print("setup done");

}

void loop() 
{

 
  
     Batt.publishToTopic();
     delay(300);
  
     client.loop();
   
    

    
}

