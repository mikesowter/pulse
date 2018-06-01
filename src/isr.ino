#include <Arduino.h>

void intServer() {
  if (!overFlow) {
    noInterrupts();
    ledState=digitalRead(LDR);
/*    delayMicroseconds(1000);      // 1ms debounce period
    if (ledState==digitalRead(LDR)) {   // valid change */
      digitalWrite(GRN,1-ledState);     // meter led on
      intBuff[intPtr] = millis();
      if (intPtr >= ISR_CAP-2) {
        setWhite();
        overFlow = 1;
      }
      intBuff[++intPtr] = 0;
    }
    interrupts();
//  }
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
    WiFi.disconnect();
    ESP.restart();
  }
  interrupts();
}
