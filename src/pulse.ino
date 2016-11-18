#include <Arduino.h>



/*
  Pulse:  reads 1kW pulses from the energy meter, determines an average power from rise and fall times
  and counts pulses to give remote metering of imported electrical energy
  A web service provides a 3 hour window of activity which can be scrolled back over the previous 24 hours
  Hourly power activity is stored by the day in flash, and then FTP'd at midnight
  Hourly energy data is transferred monthly.

  Mike Sowter  March 2016

*/
#include "pulse.h"

void setup() {
  flashLEDs();

  Serial.begin(115200);
  Serial.println("\nPulse Reader Version 2.0  2016-11-16");
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

  Serial.println("local IP address: ");
  Serial.print(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("    signal strength ");
  Serial.print(rssi);
  Serial.println(" dBm");

  udp.begin(localPort);

  // Set epoch and timers
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPrequest(timeServerIP);
  startSeconds=getNTPreply();
  setTime(startSeconds);
  //setTime(23,59,45,1,11,2016);
  startMillis = millis();
  oldMin = minute();
  oldHour = hour();
  oldDay = day();
  oldMonth = month();
  oldYear = year();
  clockDisplay();

  //if(!SPIFFS.format()||!SPIFFS.begin())     //use to format SPIFFS drive
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS failed");
  }
  SPIFFS.info(fs_info);
  Serial.print(fs_info.totalBytes);
  Serial.println(" bytes available");
  Serial.print(fs_info.usedBytes);
  Serial.println(" bytes used:");
  readFiles();

  t0 = millis();
  secondTick.attach(1,ISRwatchDog);
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
  digitalWrite(BLU,1);      //turn off Blue
}

void loop() {
  while (intPtr == 0) {
    if ( minute()!=oldMin ) {
      minProc();
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

void flashLEDs() {
  pinMode(BLU, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(LDR, INPUT);
  digitalWrite(BLU,0);      //turn on Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,1);      //turn off Red
  delay(300);
  digitalWrite(BLU,1);      //turn off Blue
  digitalWrite(GRN,0);      //turn on Green
  digitalWrite(RED,1);      //turn off Red
  delay(300);
  digitalWrite(BLU,1);      //turn off Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,0);      //turn on Red
  delay(300);
  digitalWrite(BLU,0);      //turn on Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,1);      //turn off Red
}
