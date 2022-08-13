#include "datatypes.h"
#include "arduino.h"



void initBattery()
{

}

classCell::classCell(int channel, ADS1115 *ADC)
    {
    this->faktor = 1;    
    this->ADC = ADC;
    this->channel = channel;
    ADC->begin();
    ADC->setGain(0);
    ADC->setDataRate(7);
    }

classCell::classCell(int channel, ADS1115 *ADC, double faktor)
    {
    this->faktor = 1; 
 
    this->ADC = ADC;
    this->channel = channel;
    ADC->begin();
    ADC->setGain(0);
    ADC->setDataRate(7);
    }

classCell::classCell(int channel, ADS1115 *ADC, double faktor, int gain, int datarate)
    {
    this->faktor = 1; 
    this->gain = gain;
    this->datarate = datarate;   
    this->ADC = ADC;
    this->channel = channel;
    ADC->begin();
    ADC->setGain(gain);
    ADC->setDataRate(datarate);
    }

void classCell::publishToTopic()
    {
    double result = PrettyPrintResults();
    }

double classCell::PrettyPrintResults()
    {
    int result = ADC->readADC(channel);
    double fresult = (double)result * faktor;
    Serial.printf("cell0%d: %f\n",channel, fresult);
    return fresult;
    }