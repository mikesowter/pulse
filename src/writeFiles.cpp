#include <arduino.h>
#include <fs.h>

void diagMess(const char* mess);
char* i2sd(uint8_t b);
char* f2s2(float f);
char* timeStamp();

extern File fh;
extern char charBuf[],fileName[],todayName[];
extern double emT11Energy, emT31Energy;

uint8_t storeData() {
  fh = SPIFFS.open(todayName,"a+");
  fh.printf("%s,,%.1f,%.1f\n",timeStamp(),emT11Energy,emT31Energy);
  fh.close();
  return 1;
}
