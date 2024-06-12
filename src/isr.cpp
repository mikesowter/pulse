#include <Arduino.h>
#include <constants.h>
#include <LittleFS.h>

void diagMess(const char* mess);

extern volatile uint32_t intBuff[];
extern volatile int watchDog;
extern volatile byte intPtr;
extern volatile bool overFlow;
extern File fd,fe;
extern uint8_t ledState;
extern char charBuf[];
extern volatile bool bounce;

void errMess(const char* mess);
void setRed();


IRAM_ATTR void intServer() {
/*  if ( !overFlow ) {
    intBuff[intPtr] = millis();
    intBuff[++intPtr] = 0;
    if ( intPtr >= ISR_CAP ) overFlow = true;
  }
} */
 
  if (!overFlow) {
    noInterrupts();

/*    uint32_t us = micros();
    bounce = false;
    ledState = digitalRead(LDR);
    while ( micros() - us < 5000 ) {         // 5ms debounce period
      if ( ledState != digitalRead(LDR) ) bounce = true;
    }        
    if ( !bounce ) {                         // valid change  */

    ledState = digitalRead(LDR);
    digitalWrite(GRN,1-ledState);    
    intBuff[intPtr] = millis();
    if (intPtr >= ISR_CAP-2) {
      setRed();
      overFlow = 1;
    }
    intBuff[++intPtr] = 0;
//    }
    interrupts();
  }
}   

void ISRwatchDog () {
  watchDog++;
	if (watchDog == 15) {
    errMess("watchDog 15s alert");
    setRed();
  }
  else if (watchDog >= 60) {
    errMess("watchDog 60s timeout");
    fd.close();
    fe.close();
    ESP.restart();
  }
}
