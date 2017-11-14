
/*-------- FTP session control ----------*/

byte uploadMonth() {
  strcpy(fileName,"/EN");
  strcat(fileName,p2d(oldYear/100));
  strcat(fileName,p2d(oldYear%100));
  strcat(fileName,p2d(oldMonth));
  strcat(fileName,".csv");
  uploadFile();
}

byte uploadDay() {
  strcpy(fileName,todayName);
  uploadFile();
}

byte uploadFile() {
  if (FTP_busy) return 0;
  FTP_busy = true;
  watchDog=0;
  yield();
  for (int trial=0;trial<3;trial++) {
    if (openFTPsession(fileServerIP)!=0) break;
      errMessage("FTP open session failed");
      if (trial==2) return 0;
      delay(5000);
    }
  /*  read the SPIFFS directory
  Serial.println("reading directory of flash:");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    fileName = dir.fileName();  */

  fl = SPIFFS.open(fileName, "r");
  if (!fd) {
    fd.println("FTP file open failed");
    fd.close();
    FTP_busy = false;
    return 0;
  }
  fd.print(fileName);
  fd.println(" open");
  if (!fl.seek((uint32_t)0, SeekSet)) {
    fd.println("FTP Rewind failed");
    fl.close();
    fd.close();
    FTP_busy = false;
    return 0;
  }

//  advise if sent successfully
  if (sendFile()==1) {
    fd.print(fileName);
    fd.println(" sent");
  }
  yield();
  fl.close();
  FTP_busy = false;

  //  close FTP session
  if (closeFTPsession()==0) {
    errMessage("FTP close failed");
    return 0;
  }
  return 1;
}

byte openFTPsession(IPAddress& address) {
  if (client.connect(address, 21)) {
    fd.println("FTP server connected");
  } else {
    fl.close();
    errMessage("FTP connection failed");
    return 0;
  }
  fd.println(" Sending USERNAME");
  client.println("USER pulse@sowter.com");
  if (!ftpRcv()) return 0;
  yield();
  fd.println(" Sending PASSWORD");
  client.println("PASS LovelyRita");

  if (!ftpRcv()) return 0;
  yield();
  fd.println(" Sending UTF8 ON");
  client.println("OPTS UTF8 ON");
  if (!ftpRcv()) return 0;
  yield();
  fd.println(" Sending Type I");
  client.println("Type I");
  if (!ftpRcv()) return 0;
  watchDog = 0;
  yield();
 }

byte closeFTPsession() {
  client.println("QUIT");
  fd.println(" FTP completed");

  if (!ftpRcv()) return 0;
  client.stop();
  fd.println(" Command disconnected");

  return 1;
}

//-------------- FTP server response

byte ftpRcv() {
  byte respCode;
  byte thisByte;

  while (!client.available()) {
    fd.print('w');
    delay(100);
  }

  respCode = client.peek();
  fd.print(" Response Code = ");
  fd.println(respCode);

  int outCount = 0;

  while (client.available()) {
    thisByte = client.read();
    fd.write(thisByte);
/*
    if (outCount < 127) {
      outBuf[outCount] = thisByte;
      outCount++;
      outBuf[outCount] = 0;
    }   */
  }
fd.println();
return 1;
}

//------------------------ FTP fail
void efail() {
  byte thisByte = 0;

  client.println("QUIT");

  while (!client.available()) yield();

  while (client.available()) {
    thisByte = client.read();
    fd.write(thisByte);
  }

  client.stop();
  fd.println("\nCommand disconnected");
  fl.close();
  Serial.println("file closed");
}  // efail
