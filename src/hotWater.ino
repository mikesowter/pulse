#include <arduino.h>
#include <ESP8266WiFi.h>

char host[] = "192.168.1.56";   // RMS slave

void hotWater()
{
  WiFiClient client;
  char buff[5];
  char getStr[] = "GET /hotwater HTTP/1.1\r\nHost: 192.168.1.56\r\nConnection: close\r\n\r\n";

  if (client.connect(host, 80))
  {
    client.write(getStr,73);

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        char ch = client.read();
        Serial.print(ch);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(10);
}