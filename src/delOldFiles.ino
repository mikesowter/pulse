void delOldFiles() {
  // delete all files more than 15 days old
  long SECS_15_DAYS = 15*24*60*60;
  strcpy(date15Back,"/PR");
  strcat(date15Back,p2d(year(now()-SECS_15_DAYS)%100));
  strcat(date15Back,p2d(month(now()-SECS_15_DAYS)));
  strcat(date15Back,p2d(day(now()-SECS_15_DAYS)));
  strcat(date15Back,".csv");

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    Serial.print(fileName);
    Serial.print("\t");
  //    checkFileSent();
    if (strcmp(fileName,date15Back)<0 && fileName[1]=='P') {
      Serial.print("Remove old files:\n");
      if (SPIFFS.remove(fileName)) {
        strcpy(outBuf,fileName);
        strcat(outBuf," deleted");
        errMessage(outBuf);
      }
    }
    else {
      Serial.println(dir.fileSize());
    }
  }
}
