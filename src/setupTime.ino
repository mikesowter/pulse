void setupTime() {
  startSeconds=getTime();
  setTime(startSeconds);
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  t0 = millis();
  oldMin = minute();
  oldHour = hour();
  if (hour()<7||hour()>=22) T31time = true;
  else T31time = false;
  strcpy(todayName,"/PR");
  strcat(todayName,dateStamp());
  strcat(todayName,".csv");
}
