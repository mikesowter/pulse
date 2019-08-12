// store power and energy logs

char* f2s2(float f);
char* i2sd(uint8_t b);

byte storeData() {
  // energy data
  strcpy(fileName,"/Energy.csv");
  fl = SPIFFS.open(fileName, "w");
  fl.print(i2sd(day()));
  fl.print(",");
  fl.print(i2sd(hour()));
  fl.print(",");
  fl.print(i2sd(minute()));
  fl.print(",");
  fl.print(f2s2(emT11Energy));
  fl.print(",");
  fl.println(f2s2(emT31Energy));
  fl.close();
  return 1;
}
