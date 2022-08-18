#ifndef datatypes_h
#define datatypes_h
#include "ADS1X15.h"
#include "PubSubClient.h"
#include "utils.h"
#include "Battery_heater.h"

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
    float cellvoltage[BATTERY_CELLCNT];   
public:
    bool StateCharge;
    bool StateDischarge;
    bool StateHeater;
    bool StateESP;

    void setup();
    classBattery();
    float getCellVoltage(int cellnumber);
    classCell *cell[BATTERY_CELLCNT];
    Battery_heater *Heater;
    void update();

};

extern ADS1115 ADS0;      //addr -> vcc
extern ADS1115 ADS1;      //addr -> gnd

/* User-Klassen für jeden Messkanal initalisieren */
extern classCell cell01;
extern classCell cell02;
extern classCell cell03;
extern classCell cell04;

extern classCell cell05;
extern classCell cell06;
extern classCell cell07;
extern classCell cell08;


#endif