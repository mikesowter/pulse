#include <arduino.h>
#include <ESP8266WiFi.h>

char host[] = "192.168.1.56";   // RMS slave

void hotWater()
{
  WiFiClient client;
  char buff[86] = "";
  const char getStr[] = "GET /hotwater HTTP/1.1\r\nHost: 192.168.1.56\r\nConnection: close\r\n\r\n";
  uint8_t i = 0;

  if (client.connect(host, 80))
  {
    client.write(getStr,73);
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        buff[i++] = client.read();
        if ( i > 86 ) return;
      }
    }
    client.stop();
    if ( buff[83] != 'O' ) Serial.println(" H/W not On or Off");
    if ( buff[84] == 'n' ) waterOn = true;
    else waterOn = false;
  }
  else
  {
    Serial.println("hot water connection failed!]");
    client.stop();
  }
  yield();
}