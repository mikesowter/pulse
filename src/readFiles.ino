//  open files to restore progress after restart

byte readLogs() {
  // energy data
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  fl=openFile(fileName,"r");
  if (fl) {
    readEnergy();
    fl.close();
  }
}

byte readPower() {
  byte hh,mm;
  Serial.println();
  while (fl.available()) {
    hh = fl.parseInt();
    mm = fl.parseInt();
    logData.hi = fl.parseFloat();
    logData.lo = fl.parseFloat();
  }
  return 1;
}

byte readEnergy() {
  byte dd,hh,mm;
  float eNew = 0.0;
  Serial.println();
  while (fl.available()) {
    dd = fl.parseInt();
    hh = fl.parseInt();
    mm = fl.parseInt();
    eNew = fl.parseFloat();
    if (eNew > T11Energy) T11Energy = eNew;   // expect monotonic increase
    eNew = fl.parseFloat();
    if (eNew > T33Energy) T33Energy = eNew;   // expect monotonic increase
  }
  return 1;
}
