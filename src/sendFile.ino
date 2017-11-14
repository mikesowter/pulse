#include <Arduino.h>

//  open new data connection for each file in folder

byte sendFile() {
  Serial.println("Sending PASV");
  client.println("PASV");
  yield();

  if (!ftpRcv()) return 0;
  char *tStr = strtok(outBuf, "(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL, "(,");
    array_pasv[i] = atoi(tStr);
    if (tStr == NULL) {
      Serial.println("Bad PASV Answer");
    }
  }

  unsigned int hiPort, loPort;
  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;

  Serial.print("Data port: ");
  hiPort = hiPort | loPort;
  Serial.println(hiPort);

  if (dclient.connect(fileServerIP, hiPort)) {
    Serial.println("Data connected");
  }
  else {
    Serial.println("Data connection failed");
    dclient.stop();
    fl.close();
    return 0;
  }

  Serial.println("Sending STOR ");
  client.print("STOR ");
  client.println(fileName);
  yield();

  if (!ftpRcv()) {
    Serial.println("dclient stopped");
    dclient.stop();
    return 0;
  }

  #define bufSizeFTP 1460
  uint8_t clientBuf[bufSizeFTP];

  short clientCount = 0;
  Serial.print("buffering ");
  while (fl.available()) {
    clientBuf[clientCount] = fl.read();
    clientCount++;
    if (clientCount > (bufSizeFTP - 1)) {
      Serial.print("B ");
      dclient.write((const uint8_t *) &clientBuf[0], bufSizeFTP);
      clientCount = 0;
      delay(10);
    }
  }
  if (clientCount > 0) dclient.write((const uint8_t *) &clientBuf[0], clientCount);

  dclient.stop();
  Serial.println("Data disconnected");

  if (!ftpRcv()) return 0;
  return 1;
}
