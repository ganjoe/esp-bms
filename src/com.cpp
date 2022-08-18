#include "com.h"
#include "credentials.h"
#include "Battery_Cells.h"

WiFiClient espClient;
PubSubClient client(espClient);

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

void  MQTTinit()
{
    client.setServer("10.20.0.34", 1883);
     client.setCallback(callback);
}