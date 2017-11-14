#include <Arduino.h>

// end of minute processing

void minProc() {
  minPtr = 60*(int)oldHour+(int)oldMin;
  if ( minEnergy == T31Energy ) { // energy hasn't changed since start of this minute
    Serial.print(p2d(oldHour));
    Serial.print(":");
    Serial.println(p2d(oldMin));
    minData[minPtr].lo = 0.0;
    minData[minPtr].av = 0.0;
    minData[minPtr].hi = 0.0;
  /*
    Serial.println(p2d(oldHour)+":"+p2d(oldMin));
    float dummy = (float)random(400,600)/100.0;
    minData[minPtr].lo = 0.9*dummy;
    minData[minPtr].av = dummy;
    minData[minPtr].hi = 1.1*dummy; */
  }
  else {
    Serial.print(p2d(oldHour));
    Serial.print(":");
    Serial.print(p2d(oldMin));
    Serial.print("  Avg = ");
    avgPower=(T31Energy-minEnergy)*60;
    Serial.print(avgPower);
    Serial.print("  Max = ");
    Serial.print(maxPower);
    Serial.print("  Min = ");
    if ( minPower > avgPower) minPower = avgPower;
    Serial.println(minPower);
    minData[minPtr].lo = reason(minPower);
    minData[minPtr].av = reason(avgPower);
    minData[minPtr].hi = reason(maxPower);
  }
  if (minPtr<420||minPtr>1320) T33time = true; // 10pm to 7am}
  else T33time = false;

  // reset for new minute
  minPower = 99.9;
  maxPower = 0.0;
  power=0.0;
  minEnergy = T31Energy;
  minMillis = millis();
  if ( hour()!=oldHour ) {
    storeData();
    if ( day() != oldDay ) {
      Serial.print(" millis since last midnight = ");
      Serial.println(midNight-minMillis);
      midNight = minMillis;
      //delay(10000);   // don't overload NTP & FTP servers on hour
      uploadDay();
      uploadMonth();
      //startSeconds=getTime();
      //setTime(startSeconds);
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
        fl.println(p8d(T31Energy));
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
  // flush fault files once per minute
  fd.flush();
  fe.flush();
}
