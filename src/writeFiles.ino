// store power and energy logs

byte storeData() {
  // power data
  fl = SPIFFS.open(todayName, "a");
  WriteFile();
  fl.close();
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  fl = SPIFFS.open(fileName, "a");
  fl.print(p2d(oldDay));
  fl.print(",");
  fl.print(p2d(oldHour));
  fl.print(",");
  fl.print(p8d(T31Energy));
  fl.print(",");
  fl.println(p8d(T33Energy));
  fl.close();
  return 1;
}

void WriteFile() {
  int outPtr = 60*oldHour;
  for (int m=0;m<60;m++) {
    fl.print(p2d(oldHour));
    fl.print(":");
    fl.print(p2d(m));
    fl.print(",");
    fl.print(minData[outPtr].hi);
    fl.print(",");
    fl.print(minData[outPtr].av);
    fl.print(",");
    fl.println(minData[outPtr++].lo);
    yield();
  }
  Serial.print("hour:");
  Serial.print(p2d(oldHour));
  Serial.println(" written");
}
