#include <Arduino.h>

byte storeData() {
  // power data
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,p2d(oldDay));
  strcat(fileName,".csv");
  if ( !openFile("a") ) return 0;
  WriteFile();
  fh.close();
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  if ( !openFile("a") ) return 0;
  fh.print(p2d(oldDay));
  fh.print(",");
  fh.print(p2d(oldHour));
  fh.print(",");
  fh.print(p8d(T31Energy));
  fh.print(",");
  fh.println(p8d(T33Energy));
  fh.close();
  return 1;
}

//----------------- open file for reading or appending

byte openFile(char* s) {
  fh = SPIFFS.open(fileName, s);
  if (!fh) {
    strcpy(outBuf,fileName);
    strcat(outBuf," failed to open");
    errMessage(outBuf);
    return 0;
  }
  return 1;
}

void WriteFile() {
  int outPtr = 60*oldHour;
  for (int m=0;m<60;m++) {
    fh.print(p2d(oldHour));
    fh.print(":");
    fh.print(p2d(m));
    fh.print(",");
    fh.print(minData[outPtr].hi);
    fh.print(",");
    fh.print(minData[outPtr].av);
    fh.print(",");
    fh.println(minData[outPtr++].lo);
    yield();
  }
  Serial.print("hour:");
  Serial.print(p2d(oldHour));
  Serial.println(" written");
}
