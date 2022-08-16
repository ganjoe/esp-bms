#ifndef datatypes_h
#define datatypes_h
#include "ADS1X15.h"
#include "PubSubClient.h"
#include "utils.h"

#define BATTERY_CELLCNT 8
#define ADS_GAIN 4
#define ADS_SPEED 7

enum TOPICS
    {
    WARPKERN,   //empfangen der buttonstati
    TANKS,
    
    };


enum WARPKERN
    {
    LASTFREIGABE_AUS,   
    LASTFREIGABE_EIN,
    QUELLFREIGABE_AUS,
    QUELLFREIGABE_EIN, 
    HEISZAUTOMATIK_EIN,
    HEIZAUTOMATIK_AUS,
    SELBSTHALTUNG_EIN,
    SELBSTHALTUNG_AUS
    };

enum TANKS
    {
    ABPUMPEN_AUS,
    ABPUMPEN_EIN,
    ANSAUGEN_AUS,
    ANSAUGEN_EIN,
    HEIZAUTOMATIK_FRISCHWASSER_AUS,
    HEIZAUTOMATIK_FRISCHWASSER_EIN,
    HEIZAUTOMATIK_ABWASSER_AUS,
    HEIZAUTOMATIK_ABWASSER_EIN,
    };

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


public:

   
    classCell(int channel, int cellnumber, ADS1115 *ADC);
    
    double getfResults();
    
};

class classBattery
{
private:
        
public:
    bool StateCharge;
    bool StateDischarge;
    bool StateHeater;
    bool StateESP;

    classCell *cell[BATTERY_CELLCNT];

};



#endif