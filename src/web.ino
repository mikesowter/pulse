#include <Arduino.h>

void handleMetric() {
  htmlStr[0]='\0';
//  addCstring("<!DOCTYPE html><html><body>");
//  addCstring(timeStamp());
  addCstring("# TYPE curPower guage" );
  addCstring("\ncurPower ");
  addCstring(p8d(power));
  addCstring("\n# TYPE minPower guage" );
  addCstring("\nminPower ");
  addCstring(p8d(minPower));
  addCstring("\n# TYPE maxPower guage" );
  addCstring("\nmaxPower ");
  addCstring(p8d(maxPower));
  addCstring("\n# TYPE T11Energy guage" );
  addCstring("\nT11Energy ");
  addCstring(p8d(T11Energy));
  addCstring("\n# TYPE T33Energy guage" );
  addCstring("\nT33Energy ");
  addCstring(p8d(T33Energy));
  addCstring("\n# TYPE WifiSignal guage" );
  addCstring("\nWifiSignal ");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring("\n# TYPE MinProc_ms guage" );
  addCstring("\nMinProc_ms ");
  addCstring(p8d((float)minMillis));
  addCstring("\n# TYPE Flash guage" );
  addCstring("\nFlash ");
  addCstring(p8d((float)fs_info.usedBytes/1000.0));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
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
    strcpy(htmlStr,"Sending File: ");
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
    server.send ( 200, "text/plain", htmlStr );
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
  htmlStr[0]='\0';
  fd.seek(0UL,SeekSet);
  while (fd.available()) {
    int k=fd.readBytesUntil('\r',line,80);
    line[k]='\0';
    addCstring(line);
    yield();
  }
  server.send ( 200, "text/plain", htmlStr );
  return 1;
}

void listFiles() {
  char fileSize[]="999999";
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    addCstring("<P>");
    addCstring(fileName);
    addCstring("&emsp;");
    itoa(dir.fileSize(),fileSize,7);
    addCstring(fileSize);
  }
  addCstring( "<HR></body></html>" );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void helpPage() {
  htmlStr[0]='\0';
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
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}
