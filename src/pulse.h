#include <arduino.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <fs.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

ESP8266WebServer server ( 80 );
WiFiUDP udp;
WiFiClient client,dclient;
time_t getNtpTime();
FSInfo fs_info;
File fl,fd,fe,fh;          // logs, diagnostics and errors
Ticker secondTick;
volatile int watchDog = 0;

char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";
char date5Back[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char errMess[5][60];
char line[81];
unsigned long getTime();
unsigned long sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();

//char ssid[] = "TelstraCF6EC7";
//char pass[] = "meauff8qqwn9";
char ssid[] = "ZombiesAteMyBrains";
char pass[] = "iron1951";
char d2Str[] = "12";
char d8Str[] = "12345.78";

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2390;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP;
IPAddress fileServerIP;
const char* ntpServerName = "au.pool.ntp.org";
const char* ftpServerName = "ftp.sowter.com";
char titleData[60];
const int HTML_SIZE = 6500;     // allows 140 char headroom at 181 cols
char htmlStr[HTML_SIZE];        // use C strings for storage efficiency
const int NTP_PACKET_SIZE = 48;
const int BUFFER_SIZE = 128;
const int ISR_CAP = 128;
byte buffer[BUFFER_SIZE];
char outBuf[128];               // for ftpRcv and errMessage
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
bool T33time;
float power,minPower,maxPower,avgPower,minEnergy,T11Energy,T33Energy;

volatile unsigned long intBuff[ISR_CAP];
volatile byte intPtr = 0;
volatile bool overFlow = 0;
bool FTP_busy = false;
