#include <arduino.h>
#include <secrets.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266FTPServer.h>
#include <WiFiUdp.h>
#include <fs.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

ESP8266WebServer server ( 80 );
FtpServer ftpSrv;
WiFiUDP udp;
WiFiClient client;
time_t getNtpTime();
FSInfo fs_info;
File fl,fd,fe,fh;          // logs, diagnostics and errors
Ticker secondTick;
volatile int watchDog = 0;
volatile int scanFail = 0;
String resetReason = "restart: " + ESP.getResetReason();
String resetDetail = ESP.getResetInfo();

void hotWater();
void delOldFiles();

char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";

char userText[] = "/XXyymmdd.csv";
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char charBuf[128];
char line[81];
unsigned long getTime();
unsigned long sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();

char d2Str[] = "12";
char d8Str[] = "12345.78";

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2390;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP;
IPAddress fileServerIP;
IPAddress ip(192, 168, 1, 50);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);
const char* ntpServerName = "au.pool.ntp.org";
const int longStrSize = 10000;
char longStr[longStrSize];        // use C strings for storage efficiency
const int NTP_PACKET_SIZE = 48;
const int BUFFER_SIZE = 128;
const int ISR_CAP = 128;
byte buffer[BUFFER_SIZE];
char outBuf[128];               // for ftpRcv and errMessage
const int TIMEZONE = 10;
const float HOT_WATER = 3.44;
const int LDR = 4;
const int BLU = 12;
const int GRN = 13;
const int RED = 14;

struct minStruct {
  float lo;
  float hi;
} logData;

byte oldMin, oldHour, oldDay, oldMonth;
int oldYear, minPtr, htmlLen;
byte ledState;
bool T31time, waterOn;
float power, emMinPower, emMaxPower;
double oldT11Energy, emT11Energy, emT31Energy;

volatile unsigned long intBuff[ISR_CAP];
volatile byte intPtr = 0;
volatile bool overFlow = 0;
