
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


    }


double classCell::getfResults()
    {
    iResult = ADC->readADC(channel);
    fResult = ADC->toVoltage(iResult);
    fResult*=faktor;
    return fResult;
    }