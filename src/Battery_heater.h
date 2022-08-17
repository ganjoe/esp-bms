#ifndef BATTERY_HEATER_H
#define BATTERY_HEATER_H

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class Battery_heater
{
private:
    float minTemp, maxTemp;
    uint8_t Relais;
    int switchcounter;
    void setRelais(bool state);
     float Temp;
    
public:
   
    int enable;
    float getTemperature();
    Battery_heater(uint8_t Relais, float minTemp, float maxTemp);
    void setSwitchcounter(int counts);
    void update();

};


extern OneWire oneWire;
extern DallasTemperature sensors;


#endif