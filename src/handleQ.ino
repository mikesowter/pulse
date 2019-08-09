#include <Arduino.h>

void handleQueue() {
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
