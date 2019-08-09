void setupTime() {
  startSeconds=getTime();
  setTime(startSeconds);
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  oldMin = minute();
  oldHour = hour();
  oldMonth = month();
  oldYear = year();
  oldDay = day();

  strcpy(todayName,"/PR");
  strcat(todayName,dateStamp());
  strcat(todayName,".csv");
}
