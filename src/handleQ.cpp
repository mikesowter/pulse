#include <Arduino.h>
#include <constants.h>

extern volatile unsigned long intBuff[];
extern volatile byte intPtr;
extern volatile bool overFlow;
extern double emT11Energy, emT31Energy;
extern float power, emMinPower, emMaxPower;
extern bool waterOn;

void diagMess(const char* mess);

void handleQueue() {
  volatile uint32_t t0,t1;
  noInterrupts();
  if (overFlow) diagMess(" ISR Overflow");
  for (int in=0;in<ISR_CAP;in++) {
    if (intBuff[in] == 0) break;
    t1 = intBuff[in] - t0;
    t0 = intBuff[in];
    if ( t1 > 90 ) {
      power = 1800.0/(float)t1;
      if ( waterOn ) {
        emT11Energy += 0.0005*(1.0 - HOT_WATER/power);
        emT31Energy += 0.0005*(HOT_WATER/power);    // unit is kWh
      }
      else emT11Energy += 0.0005;
      if (power>emMaxPower) emMaxPower=power;
      if (power<emMinPower) emMinPower=power;
    }
  }
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
