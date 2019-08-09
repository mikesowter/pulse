#include <Arduino.h>

void handleMetric() {
  longStr[0]='\0';
  addCstring("# TYPE emCurrentPower guage" );
  addCstring("\nemCurrentPower ");
  addCstring(p8d(power));
  addCstring("\n# TYPE emMinPower guage" );
  addCstring("\nemMinPower ");
  addCstring(p8d(emMinPower));
  addCstring("\n# TYPE emMaxPower guage" );
  addCstring("\nemMaxPower ");
  addCstring(p8d(emMaxPower));
  addCstring("\n# TYPE emT11Energy guage" );
  addCstring("\nemT11Energy ");
  addCstring(p8d(emT11Energy));
  addCstring("\n# TYPE emT31Energy guage" );
  addCstring("\nemT31Energy ");
  addCstring(p8d(emT31Energy));
  addCstring("\n# TYPE emWifiSignal guage" );
  addCstring("\nemWifiSignal ");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", longStr );
  // reset min/max for new period
  emMinPower = power;
  emMaxPower = power;
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.print(timeStamp());
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMessage("User requested restart");
    fd.close();
    fe.close();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>User requested restart<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    ESP.restart();
  }
  else if (strncmp(userText,"/deldiags",9)==0) {
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a+");
    fd.println(dateStamp());
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Diagnostics deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/delerrs",9)==0) {
    SPIFFS.remove("/errmess.txt");
    fd = SPIFFS.open("/errmess.txt", "a+");
    fd.println(dateStamp());
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Error Messages deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(longStr,"Sending File: ");
    addCstring(userText);
    addCstring("\r\r");
    fh = openFile(userText, "r");

    while (fh.available()) {
      int k=fh.readBytesUntil('\r',line,80);
      line[k]='\0';
      addCstring(line);
      yield();
    }
    fh.close();
    addCstring("\r\r");
    addCstring("length of file: ");
    addCstring(p8d((float)htmlLen));
    server.send ( 200, "text/plain", longStr );
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else {
    strcpy(outBuf,userText);
    strcat(outBuf," is not a valid option");
    diagMess(outBuf);
    helpPage();
  }
  return;
}

uint8_t listDiags() {
  longStr[0]='\0';
  fd.seek(0UL,SeekSet);
  while (fd.available()) {
    int k=fd.readBytesUntil('\r',line,80);
    line[k]='\0';
    addCstring(line);
    yield();
  }
  server.send ( 200, "text/plain", longStr );
  return 1;
}

void handleDir() {
  char fileSizeStr[]="999999";
  longStr[0]='\0';
  ltoa(fs_info.usedBytes,fileSizeStr,10);
  addCstring(ltoa(fs_info.usedBytes,fileSizeStr,10));
	addCstring(" bytes used:\n");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    addCstring("\n");
    addCstring(fileName);
    addCstring("\t");
    itoa(dir.fileSize(),fileSizeStr,10);
    addCstring(fileSizeStr);
  }
  server.send ( 200, "text/plain", longStr );
  //Serial.println(longStr);
}



void helpPage() {
  longStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  addCstring("Valid options include:");
  addCstring("<P>");
  addCstring("8.3 filename");
  addCstring("<P>");
  addCstring("avg");
  addCstring("<P>");
  addCstring("deldiags");
  addCstring("<P>");
  addCstring("delerrs");
  addCstring("<P>");
  addCstring("dir");
  addCstring("<HR>");
  addCstring("metrics");
  addCstring("<P>");
  addCstring("reset");
  addCstring("<P>");
  addCstring("shutdown");
  addCstring("<P>");
  addCstring( "<HR></body></html>" );
  server.send ( 200, "text/html", longStr );
  //Serial.println(longStr);
}