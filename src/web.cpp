#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <constants.h>

extern File fd,fe;
extern FSInfo fs_info;
extern float power, emMinPower, emMaxPower;
extern double emT11Energy, emT31Energy, T11_midnight, T31_midnight;
extern char longStr[], fileSizeStr[], fileName[], userText[], charBuf[];
extern ESP8266WebServer server;
extern uint32_t fileSize, lastScan, UDPreplyUs;
extern File fh, fd, fe;
extern uint16_t longStrLen;
extern uint8_t scanSecs;
extern bool scanFail;

char* f2s3(float f);
char* i2sd(uint8_t b);
char line[80];
void addCstring(const char* s);
uint8_t listDiags();
void handleDir();
void helpPage();
void addCstring(const char* s);
void errMess(const char* mess);
void diagMess(const char* mess);
void readLogs();
char* dateStamp();
char* timeStamp();

void handleMetric() {
  scanSecs = second();
  longStr[0]='\0';
  addCstring("# TYPE emCurrentPower guage" );
  addCstring("\nemCurrentPower ");
  addCstring(f2s3(power));
  addCstring("\n# TYPE emMinPower guage" );
  addCstring("\nemMinPower ");
  addCstring(f2s3(emMinPower));
  addCstring("\n# TYPE emMaxPower guage" );
  addCstring("\nemMaxPower ");
  addCstring(f2s3(emMaxPower));
  addCstring("\n# TYPE emT11Energy guage" );
  addCstring("\nemT11Energy ");
  addCstring(f2s3(emT11Energy));
  addCstring("\n# TYPE emT31Energy guage" );
  addCstring("\nemT31Energy ");
  addCstring(f2s3(emT31Energy));
  addCstring("\n# TYPE emT11_day guage" );
  addCstring("\nemT11_day ");
  addCstring(f2s3(1000.0*(emT11Energy - T11_midnight)));
  addCstring("\n# TYPE emT31_day guage" );
  addCstring("\nemT31_day ");
  addCstring(f2s3(1000.0*(emT31Energy - T31_midnight)));
  addCstring("\n# TYPE emUDPdelay guage" );
  addCstring("\nemUDPdelay ");
  addCstring(f2s3((float)UDPreplyUs/1000.0));
  addCstring("\n# TYPE emScanSecs guage" );
  addCstring("\nemScanSecs ");
  addCstring(i2sd(scanSecs));
  addCstring("\n# TYPE emWifiSignal guage" );
  addCstring("\nemWifiSignal ");
  addCstring(f2s3(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", longStr );
  // reset for new period
  emMinPower = power;
  emMaxPower = power;
  lastScan = millis();
}

void handleNotFound() {
  server.uri().toCharArray(userText, 30);
  Serial.print(timeStamp());
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMess("User requested restart");
    fd.close();
    fe.close();
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>User requested restart<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
    delay(1000);
    ESP.restart();
  }
  else if (strncmp(userText,"/deldiags",9)==0) {
    LittleFS.remove("/diags.txt");
    fd = LittleFS.open("/diags.txt", "a+");
    diagMess("diags deleted");
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Diagnostics deleted<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
  }
  else if (strncmp(userText,"/delerrs",9)==0) {
    LittleFS.remove("/errmess.txt");
    fd = LittleFS.open("/errmess.txt", "a+");
    errMess("errors deleted");
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>Error Messages deleted<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
  }
  else if (LittleFS.exists(userText)) {
    strcpy(longStr,"File: ");
    addCstring(userText);
    addCstring("\r\r");
    fh = LittleFS.open(userText, "r");

    while (fh.available()) {
      int k=fh.readBytesUntil('\r',line,80);
      line[k]='\0';
      addCstring(line);
      yield();
    }
    fh.close();
    server.send ( 200, "text/plain", longStr );
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/update",6)==0) {
    char* tok;
    tok = strtok(userText,",");
    tok = strtok(NULL,",");
    emT11Energy = atof(tok);
    tok = strtok(NULL,",");
    emT31Energy = atof(tok);
    sprintf(charBuf,"T11: %f, T31: %f\n",emT11Energy,emT31Energy);
    diagMess(charBuf);  
    strcpy(charBuf,"<!DOCTYPE html><html><head><HR>T11 and T31 updated<HR></head></html>");
    server.send ( 200, "text/html", charBuf );
  }
  else if (strncmp(userText,"/format",7)==0) {
    fd.close();
    fe.close();
    if (!LittleFS.format()) Serial.println("LittleFS.format failed");
    LittleFS.info(fs_info);
    Serial.print(fs_info.totalBytes);
    Serial.println(" bytes available");
    Serial.print(fs_info.usedBytes);
    Serial.println(" bytes used:");
    fd = LittleFS.open("/diags.txt","a+");
    fe = LittleFS.open("/errmess.txt","a+");
    sprintf(charBuf,"reformatted,%d bytes available",fs_info.totalBytes);
    diagMess(charBuf);  
    fd.flush();
  }
  else {
    strcpy(charBuf,userText);
    strcat(charBuf," is not a valid option");
    diagMess(charBuf);
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
  LittleFS.info(fs_info);
  ltoa(fs_info.usedBytes,fileSizeStr,10);
  addCstring(ltoa(fs_info.usedBytes,fileSizeStr,10));
	addCstring(" bytes used:\n");
  Dir dir = LittleFS.openDir("/");
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
  addCstring("format");
  addCstring("<HR>");
  addCstring("metrics");
  addCstring("<P>");
  addCstring("reset");
  addCstring("<P>");
  addCstring("shutdown");
  addCstring("<P>");
  addCstring("update");
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
