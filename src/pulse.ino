#include <Arduino.h>



/*
  Pulse:  reads 1Wh pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy
  A web service provides a 3 hour window of activity or a rolling 24 hours at 192.168.1.150/day
  Hourly power activity is stored by the day in flash, and then FTP'd at midnight
  Hourly energy data is accumulated monthly.

  Mike Sowter  March 2016

*/
#include "pulse.h"

void setup() {
  flashLEDs();
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 2.4  2017-11-02");
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
  watchDog=0;
  init_OTA();

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
  Serial.print(dateStamp());
  Serial.print(" ");
  Serial.println(timeStamp());
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
  delOldFiles();
  fd=openFile("/diags.txt","a+");
  fe=openFile("/errmess.txt","a+");
  readLogs();
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
    yield();
    watchDog=0;
  }
  // new events in queue
  handleQueue();
  yield();
  watchDog=0;
  // check for OTA
  ArduinoOTA.handle();
}
