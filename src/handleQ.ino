#include <Arduino.h>

void handleQueue() {
  noInterrupts();
  for (int in=0;in<32;in++) {
    t1=intBuff[in]-t0;
    t0=intBuff[in];
    if (t1<90) {
      if (in>=intPtr-1) break;
      continue;
    }
  /*  Serial.print(t1);
    Serial.print("  ");
    Serial.print(in);
    Serial.print("/");
    Serial.print(intPtr);
    Serial.print("  ");  */
    totalEnergy += 0.0005; // unit is kWh
    power = 1800.0/(float)t1;
    if (power<0.0) {
      Serial.print("power < 0  t1=");
      Serial.print(t1);
      Serial.print(" index=");
      Serial.print(in);
      Serial.print(" intBuff = ");
      for (int nn=0;nn<16;nn++) {
        Serial.print(nn);
        Serial.print("=");
        Serial.print(intBuff[nn]);
        Serial.print("  ");
      }
      break;
    }
    if (power>maxPower) maxPower=power;
    if (power<minPower) minPower=power;
    //Serial.println(power);
    if (in>=intPtr-1) break;
  }
  intPtr = 0;
  overFlow = 0;
  interrupts();
}
