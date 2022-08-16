#include "Battery_heater.h"

Battery_heater::Battery_heater(uint8_t Relais, float minTemp, float maxTemp)
{
    this->maxTemp = maxTemp;
    this->minTemp = minTemp;
    this->Relais = Relais;
    pinMode(Relais, OUTPUT);
}

void Battery_heater::getTemperature()
{

}

void Battery_heater::setRelais(bool state)
{

}

void Battery_heater::setSwitchcounter(int count)
{
    switchcounter = count;
}

void Battery_heater::update()
{
    getTemperature();
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
