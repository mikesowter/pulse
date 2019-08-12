//  open files to restore progress after restart

char* f2s2(float f);

byte readLogs() {
  // energy data
  strcpy(fileName,"/Energy.csv");
  fl=openFile(fileName,"r");
  if (fl) {
    readEnergy();
    fl.close();
  }
  else diagMess("failed to open Energy.csv");
}

byte readEnergy() {
  byte dd,hh,mm;

  dd = fl.parseInt();
  hh = fl.parseInt();
  mm = fl.parseInt();
  emT11Energy = fl.parseFloat();
  diagMess(f2s2(emT11Energy));
  emT31Energy = fl.parseFloat();
  diagMess(f2s2(emT31Energy));

  return 1;
}
