#include <Arduino.h>

void handleQueue() {
  noInterrupts();
  if (overFlow) diagMess(" ISR Overflow");
  for (int in=0;in<ISR_CAP;in++) {
    if (intBuff[in]==0) break;
    t1=intBuff[in]-t0;
    t0=intBuff[in];
    if (t1>90) {
      power = 1800.0/(float)t1;
      if (T33time && power > 3.5) {
        T11Energy += 0.0005*(1-3.5/power);
        T33Energy += 0.0005*(3.5/power); // unit is kWh
      }
      else T11Energy += 0.0005;
      if (power>maxPower) maxPower=power;
      if (power<minPower) minPower=power;
    }
  }
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
