
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

  fl = SPIFFS.open(fileName, "r");
  if (!fl) {
    diagMess("FTP file open failed");
    FTP_busy = false;
    return 0;
  }
  fd.print(fileName);
  fd.println(" open");
  if (!fl.seek((uint32_t)0, SeekSet)) {
    diagMess("FTP Rewind failed");
    fl.close();
    FTP_busy = false;
    return 0;
  }

//  advise if sent successfully
  if (sendFile()==1) {
    diagMess(" sent");
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
    diagMess("FTP server connected");
  } else {
    fl.close();
    errMessage("FTP connection failed");
    return 0;
  }
  diagMess(" Sending USERNAME");
  client.println("USER pulse@sowter.com");
  if (!ftpRcv()) return 0;
  yield();
  diagMess(" Sending PASSWORD");
  client.println("PASS LovelyRita");

  if (!ftpRcv()) return 0;
  yield();
  diagMess(" Sending UTF8 ON");
  client.println("OPTS UTF8 ON");
  if (!ftpRcv()) return 0;
  yield();
  diagMess(" Sending Type I");
  client.println("Type I");
  if (!ftpRcv()) return 0;
  watchDog = 0;
  yield();
 }

byte closeFTPsession() {
  client.println("QUIT");
  diagMess(" FTP completed");

  if (!ftpRcv()) return 0;
  client.stop();
  diagMess(" Command disconnected");

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
  fd.print("Response Code = ");
  fd.println(respCode);

  fd.print(timeStamp());
  while (client.available()) {
    thisByte = client.read();
    fd.write(thisByte);
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
  fd.println();
  diagMess(" Command disconnected");
  fl.close();
  Serial.println("file closed");
}  // efail
