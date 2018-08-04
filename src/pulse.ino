#include <Arduino.h>



/*
  Pulse:  reads 1Wh pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy

  T31 (hotwater) energy separated from T11 by time and value > HOT_WATER threshold

  Server scraped by prometheus and data stored in crateDB

  Mike Sowter  Aug 2018

*/
#include "pulse.h"

void setup() {
  flashLEDs();
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 3.4  2018-08-04");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    setBlue();
    Serial.print(".");
    delay(500);
    allOff();
  }
  Serial.println("");
  Serial.println("local IP address: ");
  localIP=WiFi.localIP();
  Serial.print(localIP);
  long rssi = WiFi.RSSI();
  Serial.print("    signal strength ");
  Serial.print(rssi);
  Serial.println(" dBm");
  watchDog=0;
  init_OTA();

  udp.begin(localPort);
  // Resolve servers
  WiFi.hostByName(ntpServerName, timeServerIP);
  WiFi.hostByName(ftpServerName, fileServerIP);
  // Set epoch and timers
  setupTime();
  oldMonth = month();
  oldYear = year();
  oldDay = day();
  // indicate NTP access
  setGreen();

  //if(!SPIFFS.format()||!SPIFFS.begin())     //use to format SPIFFS drive
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS.begin failed");
    setRed();   // indicate SPIFFS issue
  }
  SPIFFS.info(fs_info);
  Serial.print(fs_info.totalBytes);
  Serial.println(" bytes available");
  Serial.print(fs_info.usedBytes);
  Serial.println(" bytes used:");
  delOldFiles();
  fd=openFile("/diags.txt","a+");
  fe=openFile("/errmess.txt","a+");
  diagMess("restart");       // restart messages

  readLogs();  // read energy this month

  attachInterrupt(digitalPinToInterrupt(LDR), intServer, CHANGE);
  interrupts();

  if ( MDNS.begin ( "pulse" ) ) {
		Serial.println ( "MDNS responder started" );
	}

  server.on ( "/", handleMetric );
  server.on ( "/diags", listDiags );
  server.on ( "/dir", listFiles );
  server.on ( "/metrics", handleMetric );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
  server.handleClient();
  allOff();   //indicate all's well
}

void loop() {
  if (intPtr) handleQueue();
  yield();
  if ( minute()!=oldMin ) minProc();
  yield();
  server.handleClient();
  yield();
  ArduinoOTA.handle();
  yield();
  watchDog=0;
}
