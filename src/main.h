#include <arduino.h>
#include <secrets.h>
#include <constants.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESPFTPServer.h>
#include <WiFiudp.h>
#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

#define FS_ID LittleFS

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
void initLEDS();
void setBlue();
void setGreen();
void setRed();
void setWhite();
void allOff();
void minProc();
uint8_t openFile(File fileName, const char* s);
uint8_t init_OTA();
void getLastScan();
void setupTime();
void handleMetric();
void handleNotFound();
void handleDir();
uint8_t listDiags();
void addCstring(const char* s);

char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";

char userText[30];
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
char charBuf[128];
uint32_t getTime();
uint32_t sendNTPrequest(IPAddress& address);
uint32_t getNTPreply();

uint32_t t0, t1, startMillis, startSeconds, UDPreplyUs;
uint32_t importWh = 0;
unsigned int localPort = 5234;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP;
IPAddress fileServerIP;
IPAddress ip(192, 168, 1, 50);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

char longStr[longStrSize];        // use C strings for storage efficiency
uint8_t buffer[BUFFER_SIZE];

struct minStruct {
  float lo;
  float hi;
} logData;

uint8_t oldSec, oldMin, oldHour, oldDay, oldMonth;
int oldYear, minPtr, htmlLen;
uint8_t ledState;
uint8_t scanSecs;
bool T31time, waterOn;
float power, emMinPower, emMaxPower;
double oldT11Energy, emT11Energy, emT31Energy, T11_midnight, T31_midnight;

volatile uint32_t intBuff[128];
volatile uint8_t intPtr = 0;
volatile bool overFlow = 0;
volatile bool bounce;