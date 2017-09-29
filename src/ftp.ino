#include <Arduino.h>

/*-------- FTP session control ----------*/

byte uploadDay() {
  strcpy(fileName,"/PR");
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,p2d(oldDay));
  strcat(fileName,".csv");
  uploadFile();
}

byte uploadMonth() {
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  uploadFile();
}

byte checkFileSent() {
  allOff();
  uploadFile();
  delay(5);
  setGreen();
}

byte uploadFile() {
  watchDog=0;
  delay(5);
  if (openFTPsession(fileServerIP)==0) {
    errMessage("FTP open session failed");
    setWhite();
    return 0;
  }
  /*  read the SPIFFS directory
  Serial.println("reading directory of flash:");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    fileName = dir.fileName();  */

  fh = SPIFFS.open(fileName, "r");
  if (!fh) {
    errMessage("FTP file open failed");
    return 0;
  }
  Serial.println(fileName);
  Serial.print(" opened");
  if (!fh.seek((uint32_t)0, SeekSet)) {
    errMessage("FTP Rewind failed");
    fh.close();
    return 0;
  }

//  advise if sent successfully
  if (sendFile()==1) {
    strcpy(outBuf,fileName);
    strcat(outBuf," sent");
    errMessage(outBuf);
  }
  delay(5);
  fh.close();

  //  close FTP session
  if (closeFTPsession()==0) {
    errMessage("FTP close failed");
    return 0;
  }
  return 1;
}

byte openFTPsession(IPAddress& address) {
  if (client.connect(address, 21)) {
    Serial.println("FTP server connected");
  } else {
    fh.close();
    errMessage("FTP connection failed");
    return 0;
  }
  if (localIP[3]==150) {    // send target hardware data to pulse FTP account
    Serial.println("Sending USERNAME");
    client.println("USER pulse@sowter.com");
    if (!ftpRcv()) return 0;
    yield();
    Serial.println("Sending PASSWORD");
    client.println("PASS LovelyRita");
  }
  else {                    // otherwise send to dev account
    Serial.println("Sending USERNAME");
    client.println("USER dev@sowter.com");
    if (!ftpRcv()) return 0;
    yield();
    Serial.println("Sending PASSWORD");
    client.println("PASS develop");
  }

  if (!ftpRcv()) return 0;
  yield();
  Serial.println("Sending UTF8 ON");
  client.println("OPTS UTF8 ON");
  if (!ftpRcv()) return 0;
  yield();
  Serial.println("Sending Type I");
  client.println("Type I");
  if (!ftpRcv()) return 0;
  yield();
 }

byte closeFTPsession() {
  Serial.println("FTP completed");
  client.println("QUIT");

  if (!ftpRcv()) return 0;
  client.stop();
  Serial.println("Command disconnected");

  return 1;
}

//-------------- FTP server response

byte ftpRcv() {
  byte respCode;
  byte thisByte;

  while (!client.available()) {
    Serial.write('-');
    delay(500);
  }

  respCode = client.peek();
  Serial.print(" Response Code = ");
  Serial.println(respCode);

  int outCount = 0;

  while (client.available()) {
    thisByte = client.read();
    Serial.write(thisByte);

    if (outCount < 127) {
      outBuf[outCount] = thisByte;
      outCount++;
      outBuf[outCount] = 0;
    }
  }
 return 1;
}

//------------------------ FTP fail
void efail() {
  byte thisByte = 0;

  client.println("QUIT");

  while (!client.available()) yield();

  while (client.available()) {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  client.stop();
  Serial.println("Command disconnected");
  fh.close();
  Serial.println("file closed");
}  // efail
