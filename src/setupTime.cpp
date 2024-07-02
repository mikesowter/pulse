#include <arduino.h>
#include <TimeLib.h>

char* dateStamp();
char* timeStamp();
unsigned long getTime();

extern uint8_t old10Sec, oldMin, old5Min, oldHour, oldDay, oldMonth;
extern uint32_t t0, startMillis;
extern char todayName[], charBuf[];

void setupTime() {
  setTime(getTime());
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  old10Sec = second()/10;
  oldMin = minute();
  old5Min = oldMin/5;
  oldHour = hour();
  oldDay = day();
  oldMonth = month();

  strcpy(todayName,"/em");
  strcat(todayName,dateStamp());
  strcat(todayName,".csv");

  Serial.printf("%s %s\n",dateStamp(),timeStamp());
}

void dayCheck() {
  if (oldDay == day()) return;
  delay(2000);   //wait 2s to clear midNight reliably
  setupTime();
  return;
}
