#include <Arduino.h>

// one minute processing

void minProc() {
  if (oldT11Energy==emT11Energy) power=0.0;
  oldT11Energy=emT11Energy;

  minMillis = millis();

  if (hour()<7||hour()>=22) T31time = true;
  else T31time = false;

  // reset for new period
  emMinPower = power;
  emMaxPower = power;

  if ( hour()!=oldHour ) {
    storeData();
    if ( day() != oldDay ) {
      delay(5000);   // don't overload NTP & FTP servers at midnight
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
  minMillis = millis()-minMillis;
}
