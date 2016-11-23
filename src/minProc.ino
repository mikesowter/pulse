#include <Arduino.h>

// end of minute processing

void minProc() {
  minPtr = 60*(int)oldHour+(int)oldMin;
  if ( minEnergy == totalEnergy ) {
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
    avgPower=(totalEnergy-minEnergy)*60;
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
  // reset for new minute
  minPower = 99.9;
  maxPower = 0.0;
  power=0.0;
  minEnergy = totalEnergy;
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
      startSeconds=getTime();
      setTime(startSeconds);
      if ( month() != oldMonth ) {
        strcpy(fileName,"EN");
        strcat(fileName,p2d(year()/100));
        strcat(fileName,p2d(year()%100));
        strcat(fileName,p2d(month()));
        strcat(fileName,".csv");
        openFile("a");
        fh.print(p2d(oldDay));
        fh.print(",");
        fh.print(p2d(oldHour));
        fh.print(",");
        fh.println(p8d(totalEnergy));
        fh.close();
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
}
