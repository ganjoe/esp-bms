
#include "arduino.h"
#include "Battery_Cells.h"
#include "com.h"



classCell::classCell(int channel,int cellnumber, ADS1115 *ADC)
    {
    this->faktor = 1;    
    this->ADC = ADC;
    this->channel = channel;
    this->cellnumber =cellnumber;
    ADC->begin();
    ADC->setGain(ADS_GAIN);
    ADC->setDataRate(ADS_SPEED);
    ADC->setMode(0);      // continuous mode

    StateCharge =       false;
    StateDischarge =    false;
    }



void classBattery::publishToTopic()
    {
    StaticJsonDocument<256> doc; // ArduinoJson6 Static Json Buffer
    char jbuffer[256];
     
    doc["cell01"] = cell[0]->getfResults();
    doc["cell02"] = cell[1]->getfResults();
    doc["cell03"] = cell[2]->getfResults();
    doc["cell04"] = cell[3]->getfResults();
    doc["cell05"] = cell[4]->getfResults();
    doc["cell06"] = cell[5]->getfResults();
    doc["cell07"] = cell[6]->getfResults();
    doc["cell08"] = cell[7]->getfResults();

    
  size_t bytes = serializeJson(doc, jbuffer);

  client.publish("warpkern_spannung", jbuffer, bytes);
 
  
 
    }
void classBattery::receiveFromTopic()
    {
        
      
      
       // client.unsubscribe("warpkern_last");
    }

void Battery_callback(char* topic, byte* payload, unsigned int length)
{
  
    Serial.print("callback");
 
 

}

double classCell::getfResults()
    {
    iResult = ADC->readADC(channel);
    fResult = ADC->toVoltage(iResult);
    fResult*=faktor;
    return fResult;
    }