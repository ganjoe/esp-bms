#include <Arduino.h>
#include "datatypes.h"


ADS1115 ADS0(0x48);      //addr -> vcc
ADS1115 ADS1(0x47);      //addr -> gnd
classCell cell01(0, &ADS0);classCell cell02(0, &ADS0);classCell cell03(0, &ADS0);classCell cell04(0, &ADS0);
classCell cell05(0, &ADS1);classCell cell06(0, &ADS1);classCell cell07(0, &ADS1);classCell cell08(0, &ADS1);

classCell *Battery[BATTERY_CELLCNT];  


void setup() 
{
Battery[0] = &cell01;
Battery[1] = &cell02;
Battery[2] = &cell03;
Battery[3] = &cell04;
Battery[4] = &cell05;
Battery[5] = &cell06;
Battery[6] = &cell07;
Battery[7] = &cell08;
}

void loop() 
{
  for (size_t i = 0; i < BATTERY_CELLCNT; i++)
    {
    Battery[i]->PrettyPrintResults();
    }
}