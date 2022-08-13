#ifndef datatypes_h
#define datatypes_h
#include "ADS1X15.h"
#include "PubSubClient.h"
#include "utils.h"

#define BATTERY_CELLCNT 8
#define ADS_GAIN 4
#define ADS_SPEED 7



class classCell
{
private:
    double faktor;
    int gain;
    int datarate;
    ADS1115 *ADC;
    int channel;
    int cellnumber;

public:
    classCell(int channel, int cellnumber, ADS1115 *ADC);
    classCell(int channel, int cellnumber,ADS1115 *ADC, double faktor);
    classCell(int channel, int cellnumber,ADS1115 *ADC, double faktor, int gain, int datarate);
    double PrettyPrintResults();
    void publishToTopic();
};

#endif