#include <Arduino.h>
#include <TimeLib.h>
#include <fs.h>

void hotWater();
void setupTime();
uint8_t storeData();
uint8_t storeEnergy();
void errMess(const char* mess);
void diagMess(const char* mess);

extern File fd,fe; 
extern uint8_t oldMin, oldHour, oldDay, oldMonth;
extern int oldYear, minPtr, htmlLen;
extern float power, emMinPower, emMaxPower;
extern double oldT11Energy, emT11Energy, emT31Energy;
extern unsigned long t0, t1;
extern char charBuf[];

// one minute processing

void minProc() {
  yield();
  // check if hot water's on
  hotWater();
  yield();
  Serial.println("minproc");
  if (oldT11Energy == emT11Energy) power = 0.0;
  oldT11Energy = emT11Energy;

  uint32_t t2 = millis();
  if ( minute()%5 == 0 ) {
    storeEnergy();                        // at 5kW gives max .4kWh error
//    sprintf(charBuf,"store took %li ms",millis() - t2);
//    diagMess(charBuf);
  }

  if ( hour() != oldHour ) {
    storeData();
    if ( day() != oldDay ) {
      if ( minute() == 0 ) return;        // don't overload NTP server at midnight
      setupTime();
      if ( month() != oldMonth ) {
        if ( year() != oldYear ) {
          errMess("Happy New Year!!!!!");
          oldYear = year();
        }
        oldMonth = month();
      }
      oldDay = day();
    }
    oldHour = hour();
  }
  oldMin = minute();
  // flush fault files
  fd.flush();
  fe.flush();
  yield();
}
