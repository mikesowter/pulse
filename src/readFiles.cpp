#include <arduino.h>
#include <fs.h>

//  open files to restore progress after restart

char* f2s2(float f);
extern File fl;
extern double emT11Energy, emT31Energy;

void readEnergy();
void diagMess(const char* mess);

void readLogs() {
  // energy data
  fl = SPIFFS.open("/Energy.csv","r");
  if (fl) {
    readEnergy();
    fl.close();
  }
  else diagMess("failed to open Energy.csv");
}

void readEnergy() {
  fl.parseInt();
  fl.parseInt();
  fl.parseInt();
  fl.parseInt();
  emT11Energy = fl.parseFloat();
//  diagMess(f2s2(emT11Energy));
  emT31Energy = fl.parseFloat();
//  diagMess(f2s2(emT31Energy));
  return;
}
