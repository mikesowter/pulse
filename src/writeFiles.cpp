#include <arduino.h>
#include <fs.h>

void diagMess(const char* mess);
char* i2sd(uint8_t b);
char* f2s2(float f);
char* timeStamp();

extern File fh,fl;
extern char charBuf[],fileName[],todayName[];
extern double emT11Energy, emT31Energy;

uint8_t storeData() {
  fh = SPIFFS.open(todayName,"a+");
  fh.printf("%s,,%.1f,%.1f\n",timeStamp(),emT11Energy,emT31Energy);
  fh.close();
  return 1;
}

uint8_t storeEnergy() {
  // latest energy data
  fl = SPIFFS.open("/Energy.csv", "w");
  fl.print(timeStamp());
  fl.print(",");
  fl.print(f2s2(emT11Energy));
  fl.print(",");
  fl.println(f2s2(emT31Energy));
  fl.close();
  return 1;
}

