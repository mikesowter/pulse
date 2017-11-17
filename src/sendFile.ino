//  open new data connection for each file in folder

byte sendFile() {
  yield();
  diagMess(" Sending PASV");
  client.println("PASV");

  if (!ftpRcv()) return 0;
  char *tStr = strtok(outBuf, "(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL, "(,");
    array_pasv[i] = atoi(tStr);
    if (tStr == NULL) {
      diagMess("Bad PASV Answer: ");
    }
    diagMess(tStr);
  }

  uint16_t port = array_pasv[4] << 8;
  port |= array_pasv[5] & 255;
  char portStr[5];

  strcpy(outBuf,"Data port: ");
  itoa(port,portStr,16);
  strcat(outBuf,portStr);
  diagMess(outBuf);

  if (dclient.connect(fileServerIP, port)) {
    diagMess("Data connected");
  }
  else {
    diagMess("Data connection failed");
    dclient.stop();
    fl.close();
    return 0;
  }

  diagMess("Sending STOR ");
  client.print("STOR ");
  client.println(fileName);
  yield();

  if (!ftpRcv()) {
    diagMess("dclient stopped");
    dclient.stop();
    return 0;
  }

  #define bufSizeFTP 1460
  uint8_t clientBuf[bufSizeFTP];

  short clientCount = 0;
  diagMess("buffering ");
  while (fl.available()) {
    clientBuf[clientCount] = fl.read();
    clientCount++;
    if (clientCount > (bufSizeFTP - 1)) {
      fd.print("B ");
      dclient.write((const uint8_t *) &clientBuf[0], bufSizeFTP);
      clientCount = 0;
      delay(10);
    }
  }
  if (clientCount > 0) dclient.write((const uint8_t *) &clientBuf[0], clientCount);

  dclient.stop();
  fd.println();

  if (!ftpRcv()) return 0;
  return 1;
}
