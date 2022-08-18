
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

classBattery::classBattery()
{
    sensors.begin(); // one wire tempsensor
     ADS0.begin();
     ADS1.begin();
}

void classBattery::setup()
{
     ADS0.begin();
     ADS1.begin();
}

float classBattery::getCellVoltage(int cellnumber)
{
    return cellvoltage[cellnumber];
}


void classBattery::update()
{
    Heater->update();
    for (size_t i = 0; i < BATTERY_CELLCNT; i++)
        {
        cellvoltage[i]= cell[i]->getfResults();
        }
    
    
}

classBattery Batt;

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