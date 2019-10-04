// send a query to the distribution board to see if the hot water is on

#include <arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiudp.h>

extern WiFiUDP udp;
unsigned int otherudpPort = 4210;  // distant port to talk to
IPAddress otherIP(192,168,1,56);
char incomingPacket[64];  
extern char charBuf[];
char sendPacket[] = "How's the water?";  // a string to send 
extern bool waterOn;
extern uint32_t UDPreplyUs;
void diagMess(const char* mess);

void hotWater()
{
  uint32_t t0,t1;
  t0 = micros();
  // send a request to RMS slave to see if hot water is on
  udp.beginPacket(otherIP, otherudpPort);
  udp.write(sendPacket);
  udp.endPacket();
  t1 = micros();
  // wait for 10ms for reply
  // while ( micros() - t1 < 100000 ) {
  delay(10);
  if ( udp.parsePacket() ) {
    // receive incoming udp packets
    int len = udp.read(incomingPacket, 64);
    if (len > 0) incomingPacket[len] = 0;
    // Serial.println(incomingPacket);
    waterOn = ( incomingPacket[5] == 'o' && incomingPacket[6] == 'n' );
  }
  else diagMess("no UDP reply");
  // UDPreplyUs = micros() - t1;
}