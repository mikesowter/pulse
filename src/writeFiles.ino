// store power and energy logs

byte storeData() {
  // power data
  fl = SPIFFS.open(todayName, "a");
  fl.print(p2d(oldHour));
  fl.print(":");
  fl.print(p2d(oldMin));
  fl.print(",");
  fl.print(minData[minPtr].hi);
  fl.print(",");
  fl.print(minData[minPtr].av);
  fl.print(",");
  fl.println(minData[minPtr].lo);
  yield();
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
  fl.print(p2d(oldMin));
  fl.print(",");
  fl.print(p8d(T11Energy));
  fl.print(",");
  fl.println(p8d(T33Energy));
  fl.close();
  return 1;
}
