#include <arduino.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
extern "C" {
#include "user_interface.h"
}

ESP8266WebServer server ( 80 );
WiFiUDP udp;
WiFiClient client,dclient;
time_t getNtpTime();
FSInfo fs_info;
File fh;
Ticker secondTick;
volatile int watchDog = 0;

char fileName[] = "/XXyymmdd.csv";
char weekName[] = "/XXyymmdd.csv";
char errMess[5][60];
unsigned long getTime();
unsigned long sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();

char ssid[] = "ZombiesAteMyBrains";  //  your network SSID (name)
//char ssid[] = "DownWithPlants";
char pass[] = "iron1951";        //  your network password
char d2Str[] = "12";
char d8Str[] = "12345.78";

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2390;   //  a random local port to listen for UDP packets

IPAddress timeServerIP,fileServerIP;
const char* ntpServerName = "time.nist.gov";
const char* ftpServerName = "ftp.sowter.com";
char titleData[60];
const int HTML_SIZE = 6500;     // allows 140 char headroom at 181 cols
char htmlStr[HTML_SIZE];        // use C strings for storage efficiency
const int NTP_PACKET_SIZE = 48;
const int BUFFER_SIZE = 128;
byte Buffer[BUFFER_SIZE];
char outBuf[128];               // for ftpRcv
const int timeZone = 10;
const int LDR = 14;
const int BLU = 12;
const int GRN = 5;
const int RED = 4;
const int numCols = 181;

struct minStruct {
  float lo;
  float av;
  float hi;
} minData[1440];

byte oldMin,oldHour,oldDay,oldMonth;
int oldYear,minPtr,htmlLen;
byte ledState;
float power,minPower,maxPower,avgPower,minEnergy,totalEnergy;

volatile unsigned long intBuff[32];
volatile byte intPtr = 0;
volatile bool overFlow = 0;
