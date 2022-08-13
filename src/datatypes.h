#ifndef datatypes_h
#define datatypes_h
#include "ADS1X15.h"

#define BATTERY_CELLCNT 8

class classCell
{
private:
double faktor;
int gain;
int datarate;
ADS1115 *ADC;
int channel;

public:
    classCell(int channel, ADS1115 *ADC);
    classCell(int channel, ADS1115 *ADC, double faktor);
    classCell(int channel, ADS1115 *ADC, double faktor, int gain, int datarate);
    double PrettyPrintResults();
    void publishToTopic();
    
};




#endif