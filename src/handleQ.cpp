#include <Arduino.h>
#include <constants.h>

extern volatile unsigned long intBuff[];
extern volatile byte intPtr;
extern volatile bool overFlow;
extern double emT11Energy, emT31Energy;
extern float power, emMinPower, emMaxPower;
extern bool waterOn;
extern uint32_t t0,t1;
extern char charBuf[];

void diagMess(const char* mess);

void handleQueue() {
  if (overFlow) diagMess(" ISR Overflow");
  for (int in = 0; in < ISR_CAP; in++) {
    noInterrupts();
    if (intBuff[in] != 0) {
      interrupts();
      t1 = intBuff[in] - t0;
      t0 = intBuff[in];
      power = 1800.0/(float)t1;
      if ( power < 20000.0 ) {
        if ( waterOn ) {
          float capOne = min(1.0,HOT_WATER/power);    // energy inflow > 0
          emT11Energy += 0.0005*(1.0 - capOne); 
          emT31Energy += 0.0005*capOne;               // unit is kWh
        }
        else emT11Energy += 0.0005;
        if ( power > emMaxPower ) emMaxPower=power;
        if ( power < emMinPower ) emMinPower=power;
      }
      else {
        sprintf(charBuf," excessive power %fkW ",power);
        diagMess(charBuf);
      }
    }
    else break;
  }
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
