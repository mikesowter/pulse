#include <Arduino.h>

void handleQueue() {
  noInterrupts();
  for (int in=0;in<64;in++) {
    t1=intBuff[in]-t0;
    t0=intBuff[in];
    if (t1<90) {
      if (in>=intPtr-1) break;
      continue;
    }

    power = 1800.0/(float)t1;

    if (T33time && power > 3.5) {
      T31Energy += 0.0005*(1-3.5/power);
      T33Energy += 0.0005*(3.5/power); // unit is kWh
    }
    else T31Energy += 0.0005;

    if (power>maxPower) maxPower=power;
    if (power<minPower) minPower=power;
    //Serial.println(power);
    if (in>=intPtr-1) break;
  }
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
