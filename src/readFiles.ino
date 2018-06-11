//  open files to restore progress after restart

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
  T11Energy = fl.parseFloat();
  diagMess(p8d(T11Energy));
  T31Energy = fl.parseFloat();
  diagMess(p8d(T31Energy));

  return 1;
}
