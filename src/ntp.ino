#include <Arduino.h>

unsigned long getTime() {

  unsigned long year2030 = 1893456000UL;
  unsigned long year2017 = 1483207200UL;

  while(1) {
    while (udp.parsePacket()!= NTP_PACKET_SIZE) {
      sendNTPrequest(timeServerIP);
      delay(1000);
    }
    startSeconds = getNTPreply();
    yield();
    if (startSeconds > year2017 && startSeconds < year2030) break;
  }
  return startSeconds;
}

// send an NTP request to the time server at the given address
unsigned long sendNTPrequest(IPAddress& address)
{
  memset(buffer, 0, BUFFER_SIZE);
  // Initialize values needed to form NTP request
  buffer[0] = 0b11100011;   // LI, Version, Mode
  buffer[1] = 0;     // Stratum, or type of clock
  buffer[2] = 6;     // Polling Interval
  buffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  buffer[12]  = 49;
  buffer[13]  = 0x4E;
  buffer[14]  = 49;
  buffer[15]  = 52;
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(buffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

unsigned long getNTPreply(){
  udp.read(buffer, NTP_PACKET_SIZE); // read the packet into the buffer
  unsigned long highWord = word(buffer[40], buffer[41]);
  unsigned long lowWord = word(buffer[42], buffer[43]);
  // this is NTP time :
  unsigned long secsSince1900 = highWord << 16 | lowWord;   // seconds since Jan 1 1900
  // now convert NTP time into Unix time:
  unsigned long GMT = secsSince1900 - 2208988800UL;         // seconds since Jan 1 1970
  //add timezone offset and return
  return GMT + timeZone*3600;
}
