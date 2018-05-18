void delOldFiles() {
  // delete all files more than 30 days old
  long SECS_30_DAYS = 30*24*60*60;
  strcpy(date5Back,"/PR");
  strcat(date5Back,p2d(year(now()-SECS_30_DAYS)%100));
  strcat(date5Back,p2d(month(now()-SECS_30_DAYS)));
  strcat(date5Back,p2d(day(now()-SECS_30_DAYS)));
  strcat(date5Back,".csv");

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    Serial.print(fileName);
    Serial.print("\t");
  //    checkFileSent();
    if (strcmp(fileName,date5Back)<0 && fileName[1]=='P') {
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
