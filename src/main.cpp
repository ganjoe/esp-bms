#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Battery_Cells.h"
#include "com.h"
#include "credentials.h"

/* Ausgänge für Relais */

#define SELBSTHALTUNG D5
#define HEATERRELAIS D8
#define BATTPROTECT_LOAD D6   //BP0
#define BATTPROTECT_CHARGE D7
#define DISCONNECT D3

classBattery Batt;   

Battery_heater heater(HEATERRELAIS, 29.0, 29.5);



void setup_wifi() 
     {
     delay(10);
     Serial.println();
     Serial.print("Connecting to ");
     Serial.println(WLAN_IP);
     WiFi.mode(WIFI_STA);
     WiFi.begin("Level5", "Hallogaga#1");
     while (WiFi.status() != WL_CONNECTED) 
     {
     delay(500);
     Serial.print(".");
     }
     randomSeed(micros());
     Serial.println("");
     Serial.println("WiFi connected");
     Serial.println("IP address: ");
     Serial.println(WiFi.localIP());
     }

void reconnect() {
 
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Warpkern";


    if (client.connect(clientId.c_str())) 
    {
     Serial.println("connected");
     client.publish("debug", "hello world");
     client.subscribe("warpkern");
     client.subscribe("tanks");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* callback wird aufgerufen nachdem irgentwas vom mqtt empfangen wurde */
void callback(char* topic, byte* payload, unsigned int length) 
     {
      /*-------------debug-----------------------*/
     char msgarray[length+1];
     float fvalue = 0;
     Serial.print("Message arrived [");
     Serial.print(topic);
     Serial.print("] ");
     Serial.println(length);
     for (unsigned int i = 0; i < length; i++) 
          {
          Serial.print((char)payload[i]);
          msgarray[i] = payload[i];
          }
          msgarray[length+1] = '\0';

     Serial.println();
     
     fvalue = atof(msgarray);
     
/*---------------/debug------------------*/
 

     switch ((int)fvalue)
     {
      case LASTFREIGABE_AUS: Batt.StateDischarge = false; break;
      case LASTFREIGABE_EIN: Batt.StateDischarge = true; break;
      case QUELLFREIGABE_AUS: Batt.StateDischarge = false; break;
      case QUELLFREIGABE_EIN: Batt.StateDischarge = true; break;
      case HEISZAUTOMATIK_EIN: Batt.Heater->enable = true; break;
      case HEIZAUTOMATIK_AUS: Batt.Heater->enable = false; break;
      case SELBSTHALTUNG_EIN: Batt.StateESP = true; break;
      case SELBSTHALTUNG_AUS: Batt.StateESP = false; break;
    
     default:
          {
           Batt.StateDischarge = false;
           Batt.StateCharge = false;  
           Batt.StateHeater = false; 
           Batt.StateESP = false; 
          }
          break;
     }

     }

void setup() 
     {
     
     Serial.begin(115200); 
     sensors.begin(); // one wire tempsensor
     ADS0.begin();
     ADS1.begin();

     setup_wifi();
     client.setServer("10.20.0.34", 1883);
     client.setCallback(callback);
     scan_i2c();  

     pinMode(BATTPROTECT_LOAD, OUTPUT);
     pinMode(BATTPROTECT_CHARGE, OUTPUT);
     pinMode(SELBSTHALTUNG, OUTPUT);
     pinMode(DISCONNECT,INPUT_PULLUP);


     /* Batterie-Klasse  cell-objekt-referenzen zuweisen */

     Batt.cell[0] = &cell01;
     Batt.cell[1] = &cell02;
     Batt.cell[2] = &cell03;
     Batt.cell[3] = &cell04;
     Batt.cell[4] = &cell05;
     Batt.cell[5] = &cell06;
     Batt.cell[6] = &cell07;
     Batt.cell[7] = &cell08;

/* Batterie-Klasse heater-referenz zuweisen */
     Batt.Heater = &heater;


     Serial.print("setup done");

     }

unsigned long lastMsg = 0;

void loop() 
{
   StaticJsonDocument<256> doc; // ArduinoJson6 Static Json Buffer
  char jbuffer[256];
  
  if (!client.connected())   {    reconnect();  }
  client.loop();



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



