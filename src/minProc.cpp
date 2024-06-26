#include <Arduino.h>
#include <TimeLib.h>
#include <LittleFS.h>

void hotWater();
void setupTime();
uint8_t storeData();
void errMess(const char* mess);
void diagMess(const char* mess);

extern File fd,fe; 
extern uint8_t oldMin, old5Min, oldHour, oldDay, oldMonth;
extern int oldYear, minPtr, htmlLen;
extern float power, emMinPower, emMaxPower, emAvgPower;
extern double oldT11Energy, emT11Energy, emT31Energy, T11_midnight, T31_midnight;
extern unsigned long t0, t1;
extern char charBuf[];

// one minute processing

void minProc() {
  yield();
  if (oldT11Energy == emT11Energy) power = 0.0F;
  else {
    emAvgPower = (emT11Energy - oldT11Energy)*60.0F;
    if ( emAvgPower > 20.0F ) {
      emAvgPower = 0.0F;
      power = 0.0F;
    }
  }
  oldT11Energy = emT11Energy;
  if ( old5Min != minute()/5 )  {
    storeData();
    if ( day() != oldDay ) {
      setupTime();
      T11_midnight = emT11Energy;
      T31_midnight = emT31Energy;
      if ( month() != oldMonth ) {
        if ( year() != oldYear ) {
          diagMess("Happy New Year!!!!!");
          oldYear = year();
        }
        oldMonth = month();
      }
      oldDay = day();
    }
    old5Min = minute()/5;
  }
  oldMin = minute();
  // flush fault files
  fd.flush();
  fe.flush();
}
