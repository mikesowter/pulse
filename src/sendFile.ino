//  open new data connection for each file in folder

byte sendFile() {
  yield();
  fd.println(" Sending PASV");
  client.println("PASV");

  if (!ftpRcv()) return 0;
  char *tStr = strtok(outBuf, "(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL, "(,");
    array_pasv[i] = atoi(tStr);
    if (tStr == NULL) {
      fd.println("Bad PASV Answer");
    }
  }

  uint16_t port = array_pasv[4] << 8;
  port |= array_pasv[5] & 255;
  char portStr[5];

  strcpy(outBuf,"Data port: ");
  itoa(port,portStr,16);
  strcat(outBuf,portStr);
  fd.println(outBuf);

  if (dclient.connect(fileServerIP, port)) {
    fd.println("Data connected");
  }
  else {
    fd.println("Data connection failed");
    dclient.stop();
    return 0;
  }

  fd.println("Sending STOR ");
  client.print("STOR ");
  client.println(fileName);
  yield();

  if (!ftpRcv()) {
    fd.println("dclient stopped");
    dclient.stop();
    return 0;
  }

  #define bufSizeFTP 1460
  uint8_t clientBuf[bufSizeFTP];

  short clientCount = 0;
  fd.print("buffering ");
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
