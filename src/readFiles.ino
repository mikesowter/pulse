#include <Arduino.h>

//  open files to restore progress after restart

byte readFiles() {

  // delete all files more than 1 week old
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(year(now()-SECS_PER_WEEK)%100));
  strcat(fileName,p2d(month(now()-SECS_PER_WEEK)));
  strcat(fileName,p2d(day(now()-SECS_PER_WEEK)));
  strcat(fileName,".csv");

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(outBuf, 14);
    Serial.print(outBuf);
    Serial.print("\t");
    if (strcmp(outBuf,fileName)<0 && outBuf[1]!='E') {
      if (SPIFFS.remove(outBuf)) {
        Serial.println("deleted");
      }
    }
    else {
      Serial.println(dir.fileSize());
    }
  }

  // power data, start with previous day
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(year(now()-SECS_PER_DAY)%100));
  strcat(fileName,p2d(month(now()-SECS_PER_DAY)));
  strcat(fileName,p2d(day(now()-SECS_PER_DAY)));
  strcat(fileName,".csv");
  Serial.print("opening file: ");
  Serial.println(fileName);
  if ( openFile("r") ) {
    readPower();
    fh.close();
  }
  // overwrite with current day
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(year()%100));
  strcat(fileName,p2d(month()));
  strcat(fileName,p2d(day()));
  strcat(fileName,".csv");
  Serial.print("opening file: ");
  Serial.println(fileName);
  if ( openFile("r") ) {
    readPower();
    fh.close();
  }
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  Serial.print("opening file: ");
  Serial.println(fileName);
  if ( !openFile("r") ) return 0;
  readEnergy();
  fh.close();
  // error messages
  errMessage("log this restart");
  strcpy(fileName,"errmess.txt");
  if ( !openFile("r") ) return 0;
  readErrs();
  fh.close();
  return 1;
}

byte readPower() {
  byte hh,mm;
  Serial.println();
  while (fh.available()) {
    hh = fh.parseInt();
    mm = fh.parseInt();
    minData[60*hh+mm].hi = fh.parseFloat();
    minData[60*hh+mm].av = fh.parseFloat();
    minData[60*hh+mm].lo = fh.parseFloat();
  }
  return 1;
}

byte readEnergy() {
  byte dd,hh;
  float eSum,eMax,eNew = 0.0;
  Serial.println();
  while (fh.available()) {
    dd = fh.parseInt();
    hh = fh.parseInt();
    eNew = fh.parseFloat();
    if (eNew > eMax) eMax = eNew;   // expect monotonic increase
    /*
    else {
      eSum += eMax;
      eMax = eNew;      // step back, add old sum to new
      Serial.print(p2d(hh));
      Serial.print(":");
      Serial.print(p2d(mm));
      Serial.println(" lost energy");
    }
    */
  }
  totalEnergy = eMax;
  minEnergy = eMax;
  return 1;
}

byte readErrs() {
  char c[]=" ";
  int i,j,k;
  uint32_t ptrs[6];
  Serial.println();
  if (!fh.seek((uint32_t)0, SeekSet)) {
    errMessage("readErrs rewind failed");
    fh.close();
    return 0;
  }
  i=0;
  while (fh.available()) {
    i = (i+1)%6;
    while(fh.read()!='\n');
    ptrs[i]=fh.position();
  //  Serial.println(ptrs[i]);
    delay(1);
  }
  for (j=0;j<5;j++) {
    if (--i<0) i=5;
//    Serial.println(ptrs[i]);
    fh.seek(ptrs[i], SeekSet);
    strcpy(errMess[j],"");
    for (k=0;k<64;k++) {
      c[0] = fh.read();
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
