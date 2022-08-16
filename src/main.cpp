#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Battery_Cells.h"
#include "com.h"
#include "credentials.h"
#include <OneWire.h>
#include <DallasTemperature.h>

/* Treiber-Klassen für beide ADS1115 initalisieren */
ADS1115 ADS0(0x48);      //addr -> vcc
ADS1115 ADS1(0x49);      //addr -> gnd

/* Treiber - Klasse für One-Wire Temperatursensor */
// Pinlabel für ESP ! für leitungen ab 40cm pullup verwenden
const int oneWireBus = D4;          
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

/* Ausgänge für Relais */

#define SELBSTHALTUNG D5
#define HEATER D8
#define BATTPROTECT_LOAD D6   //BP0
#define BATTPROTECT_CHARGE D7
#define DISCONNECT D3


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
Battery_heater heater(HEATER, 40.0, 45.0);



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
      case HEISZAUTOMATIK_EIN: Batt.StateHeater = true; break;
      case HEIZAUTOMATIK_AUS: Batt.StateHeater = false; break;
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
     setup_wifi();
     client.setServer("10.20.0.34", 1883);
     client.setCallback(callback);
     scan_i2c();  

     pinMode(BATTPROTECT_LOAD, OUTPUT);
     pinMode(BATTPROTECT_CHARGE, OUTPUT);
     pinMode(HEATER, OUTPUT);
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

     ADS0.begin();
     ADS1.begin();

     Serial.print("setup done");

     }

unsigned long lastMsg = 0;

void loop() 
{
   StaticJsonDocument<256> doc; // ArduinoJson6 Static Json Buffer
  char jbuffer[256];
  
  if (!client.connected())   {    reconnect();  }
  client.loop();

    sensors.requestTemperatures(); 

    /* topic warpkern senden */
    doc["cell01"] = Batt.cell[0]->getfResults();
    doc["cell02"] = Batt.cell[1]->getfResults();
    doc["cell03"] = Batt.cell[2]->getfResults();
    doc["cell04"] = Batt.cell[3]->getfResults();
    doc["cell05"] = Batt.cell[4]->getfResults();
    doc["cell06"] = Batt.cell[5]->getfResults();
    doc["cell07"] = Batt.cell[6]->getfResults();
    doc["cell08"] = Batt.cell[7]->getfResults(); 
    doc["tempboden"] = sensors.getTempCByIndex(0);
    size_t bytes = serializeJson(doc, jbuffer);
    client.publish("warpkern_daten", jbuffer, bytes);


    /* GPIOS setzen */
     digitalWrite(BATTPROTECT_LOAD, Batt.StateDischarge);
     digitalWrite(BATTPROTECT_CHARGE, Batt.StateCharge);
     digitalWrite(HEATER, Batt.StateHeater);
     digitalWrite(SELBSTHALTUNG, Batt.StateESP);

     /* GPIOS lesen */
     if (!digitalRead(DISCONNECT)) //invertiert
     {
          Serial.println("disconnect");
     }
}



