#ifndef BATTERY_HEATER_H
#define BATTERY_HEATER_H

#include "Arduino.h"

class Battery_heater
{
private:
    float minTemp, maxTemp;
    uint8_t Relais;
    int switchcounter;
    void getTemperature();
    void setRelais(bool state);
    
public:
    float Temp;
    Battery_heater(uint8_t Relais, float minTemp, float maxTemp);
    void setSwitchcounter(int counts);
    void update();

};





#endif