#include <Arduino.h>
#include <constants.h>
#include <fs.h>

void diagMess(const char* mess);

extern volatile uint32_t intBuff[];
extern volatile int watchDog;
extern volatile byte intPtr;
extern volatile bool overFlow;
extern File fd,fe;
extern volatile int scanFail;
extern uint8_t ledState;

void errMess(const char* mess);
uint8_t storeData();
void setRed();


ICACHE_RAM_ATTR void intServer() {
  if (!overFlow) {
    noInterrupts();
    ledState=digitalRead(LDR);
    delayMicroseconds(1000);            // 1ms debounce period
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
    storeData();
    fd.close();
    fe.close();
    ESP.restart();
  }
  if ( ++scanFail == 90 ) errMess("60s scan failure");
  interrupts();
}
