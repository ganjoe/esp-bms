#include <Arduino.h>
#include "datatypes.h"

/* Treiber-Klassen für beide ADS1115 initalisieren */
ADS1115 ADS0(0x48);      //addr -> vcc
ADS1115 ADS1(0x49);      //addr -> gnd

/* User-Klassen für jeden Messkanal initalisieren */
classCell cell01(0, 1, &ADS0);
classCell cell02(1, 2, &ADS0);
classCell cell03(2, 3, &ADS0);
classCell cell04(3, 4, &ADS0);

classCell cell05(0, 5, &ADS1);
classCell cell06(1, 6, &ADS1);
classCell cell07(2, 7, &ADS1);
classCell cell08(3, 8, &ADS1);

/* Array für Messkanal-Klassen damit durchiteriert werden kann */
classCell *Battery[BATTERY_CELLCNT];  

void setup() 
{
Serial.begin(115200); 

scan_i2c();  

/* Messkanal-Klassen-Array mit Referenzen füllen */
Battery[0] = &cell01; Battery[1] = &cell02; Battery[2] = &cell03; Battery[3] = &cell04;
Battery[4] = &cell05; Battery[5] = &cell06;  Battery[6] = &cell07;Battery[7] = &cell08;

ADS0.begin();
ADS1.begin();

Serial.print("setup done");
}

void loop() 
{
   for (size_t i = 0; i < BATTERY_CELLCNT; i++)
     {
     Battery[i]->PrettyPrintResults();
     }
     Serial.println();

    delay(1000);
}