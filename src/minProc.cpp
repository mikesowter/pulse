#include <Arduino.h>
#include <TimeLib.h>
#include <fs.h>

void hotWater();
void setupTime();
uint8_t storeData();
void errMess(const char* mess);
void diagMess(const char* mess);

extern File fd,fe; 
extern uint8_t oldMin, oldHour, oldDay, oldMonth;
extern int oldYear, minPtr, htmlLen;
extern float power, emMinPower, emMaxPower;
extern double oldT11Energy, emT11Energy, emT31Energy;
extern float T11_day, T31_day;
extern unsigned long t0, t1;
extern char charBuf[];

// one minute processing

void minProc() {
  yield();
  Serial.println("minproc");
  if (oldT11Energy == emT11Energy) power = 0.0;
  oldT11Energy = emT11Energy;

  if ( hour() != oldHour ) {
    storeData();
    if ( day() != oldDay ) {
      setupTime();
      T11_day = 0.0;
      T31_day = 0.0;
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
