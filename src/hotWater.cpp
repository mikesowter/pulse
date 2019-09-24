// send a query to the distribution board to see if the hot water is on

#include <arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

extern WiFiUDP udp;
unsigned int otherUdpPort = 4210;  // distant port to talk to
IPAddress otherIP(192,168,1,56);
char incomingPacket[64];  
extern char charBuf[];
char sendPacket[] = "How's the water?";  // a string to send 
extern bool waterOn;
void diagMess(const char* mess);

void hotWater()
{
  uint32_t t0,t1,t2;
  t0 = micros();
  // send a request to RMS slave to see if hot water is on
  udp.beginPacket(otherIP, otherUdpPort);
  udp.write(sendPacket);
  udp.endPacket();
  t1 = micros();
  // wait for 2ms for reply
  delay(2);
  int packetSize = udp.parsePacket();
  if ( packetSize ) {
    // receive incoming udp packets
    int len = udp.read(incomingPacket, 64);
    if (len > 0) incomingPacket[len] = 0;
    t2 = micros();
    Serial.println(incomingPacket);
    if ( t2-t1 > 3000 ) {
      sprintf(charBuf," Tx delay: %u Rx delay: %u\n",t1-t0,t2-t1);
      diagMess(charBuf);
    }
    waterOn = ( incomingPacket[5] == 'o' && incomingPacket[6] == 'n' );
  }
  else diagMess("no response to hotwater query");
}