#include <Arduino.h>

// reporting interval processing

void minProc() {
  if ( oldMin%REP_INT ) {
    oldMin = minute();
    return;
  }
  avgPower=(T11Energy-oldT11Energy)*60/REP_INT;
  if ( minPower > avgPower) minPower = avgPower;
  powerData.lo = minPower;
  powerData.av = avgPower;
  powerData.hi = maxPower;

  if (hour()<7||hour()>=22) T33time = true;
  else T33time = false;

  // reset for new period
  minPower = 99.9;
  maxPower = 0.0;
  power=0.0;
  oldT11Energy = T11Energy;
  minMillis = millis();
  storeData();      // stores data every five minutes
  if ( hour()!=oldHour ) {
    if ( day() != oldDay ) {
      delay(5000);   // don't overload NTP & FTP servers at midnight
      setupTime();
      delOldFiles();
      if ( month() != oldMonth ) {
        strcpy(fileName,"EN");
        strcat(fileName,p2d(year()/100));
        strcat(fileName,p2d(year()%100));
        strcat(fileName,p2d(month()));
        strcat(fileName,".csv");
        fd.print("Creating new month file: ");
        fd.println(fileName);
        fl=SPIFFS.open(fileName,"a+");
        // write last values from previous month into new ENyyyymm.csv file
        fl.print(p2d(day()));
        fl.print(",");
        fl.print(p2d(hour()));
        fl.print(",");
        fl.println(p8d(T11Energy));
        fl.print(",");
        fl.println(p8d(T33Energy));
        fl.close();

        if ( year() != oldYear ) {
          errMessage("Happy New Year!!!!!");
          oldYear = year();
        }
        oldMonth = month();
      }
      oldDay = day();
    }
    oldHour = hour();
  } ;
  oldMin = minute();
  // flush fault files
  fd.flush();
  fe.flush();
}
