#include <arduino.h>
#include <secrets.h>
#include <constants.h>
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
void diagMess(const char* mess);
void errMess(const char* mess);
char* dateStamp();
char* timeStamp();
void handleQueue();
void ISRwatchDog ();
ICACHE_RAM_ATTR void intServer();
void flashLEDs();
void setBlue();
void setGreen();
void setRed();
void setWhite();
void allOff();
void minProc();
uint8_t openFile(File fileName, const char* s);
uint8_t init_OTA();
void readLogs();


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

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned long importWh = 0;
unsigned int localPort = 2390;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP;
IPAddress fileServerIP;
IPAddress ip(192, 168, 1, 50);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

char longStr[longStrSize];        // use C strings for storage efficiency
uint8_t buffer[BUFFER_SIZE];
char outBuf[128];               // for ftpRcv and errMess

struct minStruct {
  float lo;
  float hi;
} logData;

uint8_t oldMin, oldHour, oldDay, oldMonth;
int oldYear, minPtr, htmlLen;
uint8_t ledState;
bool T31time, waterOn;
float power, emMinPower, emMaxPower;
double oldT11Energy, emT11Energy, emT31Energy;

volatile unsigned long intBuff[128];
volatile uint8_t intPtr = 0;
volatile bool overFlow = 0;
