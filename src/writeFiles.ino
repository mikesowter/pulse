// store power and energy logs

byte storeData() {
  // energy data
  strcpy(fileName,"/Energy.csv");
  fl = SPIFFS.open(fileName, "w");
  fl.print(p2d(day()));
  fl.print(",");
  fl.print(p2d(hour()));
  fl.print(",");
  fl.print(p2d(minute()));
  fl.print(",");
  fl.print(p8d(T11Energy));
  fl.print(",");
  fl.println(p8d(T31Energy));
  fl.close();
  return 1;
}
