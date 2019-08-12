#include <Arduino.h>

ICACHE_RAM_ATTR void intServer() {
  if (!overFlow) {
    noInterrupts();
    ledState=digitalRead(LDR);
    delayMicroseconds(1000);      // 1ms debounce period
    if (ledState==digitalRead(LDR)) {   // valid change
      digitalWrite(GRN,1-ledState);     // meter led on
      intBuff[intPtr] = millis();
      if (intPtr >= ISR_CAP-2) {
        setRed();
        overFlow = 1;
      }
      intBuff[++intPtr] = 0;
    }
    interrupts();
  }
}

void ISRwatchDog () {
  noInterrupts();
  watchDog++;
	if (watchDog == 30) {
    errMess("watchDog 30s alert");
    setRed();
  }
  else if (watchDog >= 60) {
    errMess("watchDog 60s timeout");
    fd.close();
    fe.close();
    WiFi.disconnect();
    ESP.restart();
  }
  scanFail++;
  if (scanFail == 62) errMess("60s scan failure");
  interrupts();
}
