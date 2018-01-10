//  open files to restore progress after restart

byte readLogs() {
  // power data, start with previous day
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
  }
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
  // read old error messages
  if ( !openFile("/errmess.txt","r") ) return 0;
  readErrMess();
  fl.close();
  return 1;
}

byte readPower() {
  byte hh,mm;
  Serial.println();
  while (fl.available()) {
    hh = fl.parseInt();
    mm = fl.parseInt();
    minData[60*hh+mm].hi = fl.parseFloat();
    minData[60*hh+mm].av = fl.parseFloat();
    minData[60*hh+mm].lo = fl.parseFloat();
  }
  return 1;
}

byte readEnergy() {
  byte dd,hh;
  float eNew = 0.0;
  Serial.println();
  while (fl.available()) {
    dd = fl.parseInt();
    hh = fl.parseInt();
    eNew = fl.parseFloat();
    if (eNew > T11Energy) T11Energy = eNew;   // expect monotonic increase
    eNew = fl.parseFloat();
    if (eNew > T33Energy) T33Energy = eNew;   // expect monotonic increase
  }
  minEnergy = T11Energy; // energy at start of next minute to compare to T11 at end of minute
  return 1;
}

byte readErrMess() {
  char c[]=" ";
  int i,j,k;
  uint32_t ptrs[6];
  Serial.println();
  if (!fe.seek((uint32_t)0, SeekSet)) {
    errMessage("readErrs rewind failed");
    fe.close();
    fe=openFile("/errmess.txt","a+");
    return 0;
  }
  i=0;
  while (fe.available()) {
    i = (i+1)%6;
    while(fe.read()!='\n');
    ptrs[i]=fe.position();
  //  Serial.println(ptrs[i]);
    yield();
  }
  for (j=0;j<5;j++) {
    if (--i<0) i=5;
//    Serial.println(ptrs[i]);
    fe.seek(ptrs[i], SeekSet);
    strcpy(errMess[j],"");
    for (k=0;k<64;k++) {
      c[0] = fe.read();
      strcat(errMess[j],c);
      if ( c[0]=='\r') {
        while ( k++<64 ) {
          strcat(errMess[j],"\0");
        }
      }
    }
  }
  return 1;
}
