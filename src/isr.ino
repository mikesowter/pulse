#include <Arduino.h>

void intServer() {
  if (overFlow) return;
  noInterrupts();
  ledState=digitalRead(LDR);
  delayMicroseconds(5000);      // debounce period
  ledState=digitalRead(LDR);
  digitalWrite(GRN,1-ledState); // indicate activity ON=ON
  intBuff[intPtr] = millis();
  if (intPtr > 30) {
    errMessage("ISR overflow");
    overFlow = 1;
    intPtr = 31;
    return;
  }
  intBuff[++intPtr] = 0;
  interrupts();
}

void ISRwatchDog () {
  noInterrupts();
  watchDog++;
	if (watchDog == 60) {
    errMessage("watchDog 60s alert");
  }
  if (watchDog >= 90) {
    errMessage("watchDog fired");
    ESP.reset();
  }
  interrupts();
}
