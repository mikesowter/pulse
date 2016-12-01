#include <Arduino.h>



/*
  Pulse:  reads 1Wh pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy
  A web service provides a 3 hour window of activity which can be scrolled back over the previous 24 hours
  Hourly power activity is stored by the day in flash, and then FTP'd at midnight
  Hourly energy data is transferred monthly.

  Mike Sowter  March 2016

*/
#include "pulse.h"

void setup() {
  flashLEDs();
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 2.0  2016-11-24");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  setBlue();    // indicate wifi access
  Serial.println("local IP address: ");
  localIP=WiFi.localIP();
  Serial.print(localIP);
  long rssi = WiFi.RSSI();
  Serial.print("    signal strength ");
  Serial.print(rssi);
  Serial.println(" dBm");

  udp.begin(localPort);
  // Resolve servers
  WiFi.hostByName(ntpServerName, timeServerIP);
  WiFi.hostByName(ftpServerName, fileServerIP);
  // Set epoch and timers
  startSeconds=getTime();
  setTime(startSeconds);
  //setTime(23,59,30,30,11,2016);
  startMillis = millis();
  oldMin = minute();
  oldHour = hour();
  oldDay = day();
  oldMonth = month();
  oldYear = year();
  clockDisplay();
  setGreen();   // indicate NTP access

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
  readFiles();

  t0 = millis();
  attachInterrupt(digitalPinToInterrupt(LDR), intServer, CHANGE);

  if ( MDNS.begin ( "pulse" ) ) {
		Serial.println ( "MDNS responder started" );
	}

  server.on ( "/", handleRoot );
  server.on ( "/day", handleDay );
  server.on ( "/avg", handleAvg );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
  server.handleClient();
  allOff();   //indicate all's well
}

void loop() {
  while (intPtr == 0) {
    if ( minute()!=oldMin ) {
      setWhite();
      minProc();
      allOff();
    }
    server.handleClient();
    delay(10);
    watchDog=0;
  }
  // new events in queue
  handleQueue();
  delay(10);
  watchDog=0;
}
