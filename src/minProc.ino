#include <Arduino.h>

// one minute processing

void minProc() {
  yield();
  // check if hot water's on
  hotWater();
  yield();
  Serial.println("minproc");
  if (oldT11Energy == emT11Energy) power = 0.0;
  oldT11Energy = emT11Energy;

  minMillis = millis();

  if ( hour() != oldHour ) {
    storeData();
    if ( day() != oldDay ) {
      if ( minute() == 0 ) return;        // don't overload NTP server at midnight
      setupTime();
      if ( month() != oldMonth ) {
        if ( year() != oldYear ) {
          errMessage("Happy New Year!!!!!");
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
  minMillis = millis() - minMillis;
  yield();
}
