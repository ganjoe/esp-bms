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
    double  fResult;
    int iResult;
    int cellnumber;
    bool StateCharge;
    bool StateDischarge;

public:

    classCell(int channel, int cellnumber, ADS1115 *ADC);
    
    double getfResults();
    
};

class classBattery
{
private:
        
public:

    classCell *cell[BATTERY_CELLCNT];
    void publishToTopic();  
    void receiveFromTopic();
};

void Battery_callback(char* topic, byte* payload, unsigned int length);

#endif