#include <Arduino.h>
#include <constants.h>

extern volatile unsigned long intBuff[];
extern volatile byte intPtr;
extern volatile bool overFlow;
extern double emT11Energy, emT31Energy;
extern float T11_day, T31_day;
extern float power, emMinPower, emMaxPower;
extern bool waterOn, bounce;
extern uint32_t t0,t1;
extern char charBuf[];
uint8_t buffPtr;

void diagMess(const char* mess);

void handleQueue() {
  if (overFlow) diagMess(" ISR Overflow");
  for (buffPtr = 0; buffPtr < ISR_CAP; buffPtr++) {
    noInterrupts();         // prevent preemption of NULL flag
    if (intBuff[buffPtr] != 0) {
      interrupts();
      t1 = max( 1UL, intBuff[buffPtr] - t0 );
      t0 = intBuff[buffPtr];
      power = 3600.0/(float)t1;                       // power in kW
      if ( power < 20.0 ) {
        if ( waterOn ) {
          float capOne = min(1.0,HOT_WATER/power);    // energy inflow > 0
          emT11Energy += 0.001*(1.0 - capOne); 
          emT31Energy += 0.001*capOne;               // unit is kWh
        }
        else {
          emT11Energy += 0.001;
        }
        if ( power > emMaxPower ) emMaxPower=power;
        if ( power < emMinPower ) emMinPower=power;
      }
      else if ( power < 50.0 ) {
        sprintf(charBuf," %0.1fkW 0:%lu 1:%lu 2:%lu 3:%lu 4:%lu 5:%lu",power,
                intBuff[0],intBuff[1],intBuff[2],intBuff[3],intBuff[4],intBuff[5]);
        power = emMaxPower;       // limit to previous max
        diagMess(charBuf);
      }
      else power = emMaxPower;    // limit to previous max
    }
    else break;
  }
  noInterrupts();
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
