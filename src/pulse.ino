#include <Arduino.h>



/*
  Pulse:  reads 1Wh pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy

  5 minute power activity is stored by the day in flash
  5 minute energy data is accumulated monthly.

  Mike Sowter  May 2018

*/
#include "pulse.h"

void setup() {
  flashLEDs();
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 3.2  2018-05-20");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  delay(10000);

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(5000);
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
  while (intPtr == 0) {
    if ( minute()!=oldMin ) {
      setWhite();
      minProc();
      allOff();
    }
    server.handleClient();
    yield();
    watchDog=0;
    // check for OTA
    ArduinoOTA.handle();
  }
  // new events in queue
  handleQueue();
  yield();
  watchDog=0;
  // check for OTA
  ArduinoOTA.handle();
}
