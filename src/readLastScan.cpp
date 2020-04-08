#include <arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>
 
char host[] = "192.168.1.20";   // RPi-1 prometheus and influx server
extern double emT11Energy, emT31Energy;
void diagMess(const char* mess);
 
void getLastScan() {
  WiFiClient client;
  char buff[1024];
  uint16_t buffPtr, tariff, numPtr;
  uint32_t t = now()-36000;   // zulu timestamps in prometheus data
  
  char Str1[] = "GET /api/v1/query_range?query=emT";
  char Str2[] = "_1Energy&start=";   
  char Str3[] = "1569541500";
  char Str4[] = "&end=";
  char Str5[] = "1569541619";
  char Str6[] = "&step=60&timeout=10s HTTP/1.1\r\nHost: 192.168.1.20\r\nConnection: close\r\n\r\n";

  for (tariff=1; tariff<4; tariff=tariff+2) {
    strcpy(buff,Str1);
    strcat(buff,Str2);
    dtostrf((double)(t-400), 0, 0, Str3);
    strcat(buff,Str3);
    strcat(buff,Str4);
    dtostrf((double)t, 0, 0, Str5);
    strcat(buff,Str5);
    strcat(buff,Str6);

    buff[strlen(Str1)] = '0' + tariff;      //T11 and T31
    if (client.connect(host, 9090)) {
      client.write(buff,strlen(buff));
      buffPtr = 0;
      while (client.connected() || client.available()) {
        if (client.available()) {
          buff[buffPtr++] = client.read();
          if ( buffPtr > 1000 ) {
            diagMess("prometheus reply > 1000 bytes");
            break;
          }
        }
      }
      buff[buffPtr] = '\0';
      Serial.printf("\n%d bytes: \n%s\n",buffPtr,buff);
      for (numPtr = buffPtr-8; numPtr>buffPtr-18; numPtr-- ) {
        if (buff[numPtr] == '\"') {
          if (tariff == 1) emT11Energy = atof(buff+numPtr+1);
          else emT31Energy = atof(buff+numPtr+1);
        }
      }
      client.stop();
      delay(100);
    }
  }
}