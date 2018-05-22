//  open files to restore progress after restart

byte readLogs() {
/*  // power data, start with previous day
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(year(now()-SECS_PER_DAY)%100));
  strcat(fileName,p2d(month(now()-SECS_PER_DAY)));
  strcat(fileName,p2d(day(now()-SECS_PER_DAY)));
  strcat(fileName,".csv");
  fl=openFile(fileName,"r");
  if (fl) {
    readPower();
    fl.close();
  }
  // overwrite with current day
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(year()%100));
  strcat(fileName,p2d(month()));
  strcat(fileName,p2d(day()));
  strcat(fileName,".csv");
  fl=openFile(fileName,"r");
  if (fl) {
    readPower();
    fl.close();
  } */
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  fl=openFile(fileName,"r");
  if (fl) {
    readEnergy();
    fl.close();
  }
  /* read old error messages
  if ( !openFile("/errmess.txt","r") ) return 0;
  readErrMess();
  fl.close();
  return 1;   */
}

byte readPower() {
  byte hh,mm;
  Serial.println();
  while (fl.available()) {
    hh = fl.parseInt();
    mm = fl.parseInt();
    powerData.hi = fl.parseFloat();
    powerData.av = fl.parseFloat();
    powerData.lo = fl.parseFloat();
  }
  return 1;
}

byte readEnergy() {
  byte dd,hh,mm;
  float eNew = 0.0;
  Serial.println();
  while (fl.available()) {
    dd = fl.parseInt();
    hh = fl.parseInt();
    mm = fl.parseInt();
    eNew = fl.parseFloat();
    if (eNew > T11Energy) T11Energy = eNew;   // expect monotonic increase
    eNew = fl.parseFloat();
    if (eNew > T33Energy) T33Energy = eNew;   // expect monotonic increase
  }
  oldT11Energy = T11Energy;       // energy at start of next period
  return 1;
}
