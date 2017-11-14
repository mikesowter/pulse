#include <Arduino.h>

void intServer() {
  if (overFlow) return;
  noInterrupts();
  ledState=digitalRead(LDR);
  delayMicroseconds(5000);      // debounce period
  ledState=digitalRead(LDR);
  digitalWrite(GRN,1-ledState); // indicate activity ON=ON
  intBuff[intPtr] = millis();
  if (intPtr >= ISR_CAP-1) {
    errMessage("ISR overflow");
    overFlow = 1;
    intPtr = ISR_CAP-1;
    return;
  }
  intBuff[++intPtr] = 0;
  interrupts();
}

void ISRwatchDog () {
  noInterrupts();
  watchDog++;
	if (watchDog == 30) {
    errMessage("watchDog 30s alert");
    setRed();
  }
  if (watchDog >= 60) {
    errMessage("watchDog 60s timeout");
    fd.close();
    fe.close();
    ESP.restart();
  }
  interrupts();
}
