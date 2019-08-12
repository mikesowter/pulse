#include <Arduino.h>

char* f2s2(float f);
void addCstring(const char* s);

void handleMetric() {
  longStr[0]='\0';
  addCstring("# TYPE emCurrentPower guage" );
  addCstring("\nemCurrentPower ");
  addCstring(f2s2(power));
  addCstring("\n# TYPE emMinPower guage" );
  addCstring("\nemMinPower ");
  addCstring(f2s2(emMinPower));
  addCstring("\n# TYPE emMaxPower guage" );
  addCstring("\nemMaxPower ");
  addCstring(f2s2(emMaxPower));
  addCstring("\n# TYPE emT11Energy guage" );
  addCstring("\nemT11Energy ");
  addCstring(f2s2(emT11Energy));
  addCstring("\n# TYPE emT31Energy guage" );
  addCstring("\nemT31Energy ");
  addCstring(f2s2(emT31Energy));
  addCstring("\n# TYPE emWifiSignal guage" );
  addCstring("\nemWifiSignal ");
  addCstring(f2s2(-WiFi.RSSI()));
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
    errMess("User requested restart");
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
    addCstring(f2s2((float)htmlLen));
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

void addCstring(const char* s) {
  // find end of longStr
  uint16_t p,longStrLen = longStrSize;
  for (p=0;p<longStrLen;p++) {
    if ( p>longStrLen-32) {
      diagMess("longStrLen exceeded");
      break;
    }
    if (longStr[p]=='\0') {
      break;    // p now points to end of old string
    }
  }
  uint16_t q=0;
  for (;p<longStrLen;p++) {
    longStr[p]=s[q];
    if (s[q++]=='\0') break;
  }
}
