// store power and energy logs

byte storeData() {
  // power data
  fl = SPIFFS.open(todayName, "a");
  fl.print(p2d(hour()));
  fl.print(":");
  fl.print(p2d(minute()));
  fl.print(",");
  fl.print(logData.hi);
  fl.print(",");
  fl.println(logData.lo);
  yield();
  fl.close();
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  fl = SPIFFS.open(fileName, "a");
  fl.print(p2d(day()));
  fl.print(",");
  fl.print(p2d(hour()));
  fl.print(",");
  fl.print(p2d(minute()));
  fl.print(",");
  fl.print(p8d(T11Energy));
  fl.print(",");
  fl.println(p8d(T33Energy));
  fl.close();
  return 1;
}
