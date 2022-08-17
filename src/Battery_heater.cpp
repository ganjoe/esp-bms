#include "Battery_heater.h"

/* Treiber - Klasse für One-Wire Temperatursensor */
// Pinlabel für ESP ! für leitungen ab 40cm pullup verwenden
const int oneWireBus = D4;          
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

Battery_heater::Battery_heater(uint8_t Relais, float minTemp, float maxTemp)
{
    this->maxTemp = maxTemp;
    this->minTemp = minTemp;
    this->Relais = Relais;
    pinMode(Relais, OUTPUT);
}

float Battery_heater::getTemperature()
{
     sensors.requestTemperatures(); 
     Temp = sensors.getTempCByIndex(0);
     return Temp;
 
}

void Battery_heater::setRelais(bool state)
    {
        Serial.print("setRelais");
        Serial.println(state);
    digitalWrite(Relais, state);
    }

void Battery_heater::setSwitchcounter(int count)
{
    switchcounter = count;
}

void Battery_heater::update()
{
    getTemperature();
    if (enable)
    {
        Serial.println("heizautomatik ein");
    if (Temp > maxTemp)
        {
        setRelais(1);
        switchcounter++;
        }
    else
        {
        setRelais(0);
        }
    }
    else
    {
    setRelais(0);
    }
}
