#include <arduino.h>
#include <ESP8266WiFi.h>

char host[] = "192.168.1.56";   // RMS slave
extern bool waterOn;
void diagMess(const char* mess);

void hotWater()
{
  WiFiClient client;
  char buff[90];
  const char getStr[] = "GET /hotwater HTTP/1.1\r\nHost: 192.168.1.56\r\nConnection: close\r\n\r\n";
  uint8_t i = 0;
  uint32_t timer = millis();

  if (client.connect(host, 80))
  {
    client.write(getStr,73);
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        buff[i++] = client.read();
        if ( i > 86 ) {
          diagMess("hot water message > 86 bytes");
          break;
        }
      }
      yield();
    }
    client.stop();
    if ( buff[83] != 'O' ) diagMess(" H/W not On or Off");
    if ( buff[84] == 'n' ) waterOn = true;
    else waterOn = false;
  }
  else
  {
    diagMess("hot water connection failed!]");
    client.stop();
  }
  if (millis() - timer > 200) {
    sprintf(buff,"hot water took %lums",millis() - timer);
    diagMess(buff);
  }
}