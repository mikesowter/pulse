#include <arduino.h>
#include <TimeLib.h>

char* dateStamp();
char* timeStamp();
unsigned long getTime();

extern uint8_t oldMin,oldHour,oldDay,oldMonth;
extern uint32_t t0, startMillis;
extern char todayName[], charBuf[];

void setupTime() {
  setTime(getTime());
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  oldMin = minute();
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
