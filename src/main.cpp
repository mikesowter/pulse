#include <Arduino.h>



/*
  Pulse:  reads 1Wh pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy

  T31 (hotwater) energy separated from T11 by polling RMS slave

  Server scraped by prometheus and data stored in influxDB

  Mike Sowter  Aug 2018

*/
#include "main.h"

void setup() {
  initLEDS();
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 4.3  20230310");
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
  watchDog = 0;
  init_OTA();

  udp.begin(localPort);
  // Resolve server
  WiFi.hostByName(ntpServerName, timeServerIP);
  // Set epoch and timers
  setupTime();
  // indicate NTP success
  setGreen();

  //if (!LittleFS.format()) Serial.println("LittleFS.format failed");
  if( !LittleFS.begin() )
  {
    Serial.println("LittleFS.begin failed");
    setRed();   // indicate LittleFS issue
  }
  LittleFS.info(fs_info);
  Serial.print(fs_info.totalBytes);
  Serial.println(" bytes available");
  Serial.print(fs_info.usedBytes);
  Serial.println(" bytes used:");
  delOldFiles();
  fd = LittleFS.open("/diags.txt","a+");
  fe = LittleFS.open("/errmess.txt","a+");
  // lookup reason for restart
	resetReason.toCharArray(charBuf,resetReason.length()+1);
	diagMess(charBuf);       // restart message
	resetDetail.toCharArray(charBuf,resetDetail.length()+1);
	if ( charBuf[16] != '0' )	diagMess(charBuf);  // exception detail

  getLastScan();  // read most recent prometheus scan (up to 5 minutes ago)

  attachInterrupt(digitalPinToInterrupt(LDR), intServer, CHANGE);
  interrupts();

  if ( MDNS.begin ( "pulse" ) ) {
		Serial.println ( "MDNS responder started" );
	}

  server.on ( "/", handleMetric );
  server.on ( "/diags", listDiags );
  server.on ( "/dir", handleDir );
  server.on ( "/metrics", handleMetric );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
  server.handleClient();
  // setup FTP server
	ftpSrv.begin("mike","iron");

  allOff();   //indicate all's well
}

void loop() {
  // calculate power from pulses
  if ( intPtr ) handleQueue();
  // check for scheduled activity
  if ( minute() != oldMin ) minProc();
  // check if hot water's on
  hotWater();
  // check for web request
  server.handleClient();
  // check for OTA
  ArduinoOTA.handle();
  // check for FTP request
  ftpSrv.handleFTP(FS_ID);
  // check the network
  checkScan();
  // reset watch dog timer
  watchDog = 0;
}

void checkScan() {
  if ( millis() - lastScan > 60000UL ) {
    if (!scanFail) {
      diagMess("no scan for 60s");
      scanFail = true;
    }
  }
  else {
    if (scanFail) {
      scanFail = false;
      diagMess("scan restored");
    }
  }
}