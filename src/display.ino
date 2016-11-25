#include <Arduino.h>


/*-------- display code ----------*/

void errMessage(char* mess) {
  strcpy(errMess[4],errMess[3]);
  strcpy(errMess[3],errMess[2]);
  strcpy(errMess[2],errMess[1]);
  strcpy(errMess[1],errMess[0]);
  strcpy(errMess[0],p2d(year()%100));
  strcat(errMess[0],p2d(month()));
  strcat(errMess[0],p2d(day()));
  strcat(errMess[0]," ");
  strcat(errMess[0],p2d(hour()));
  strcat(errMess[0],":");
  strcat(errMess[0],p2d(minute()));
  strcat(errMess[0],":");
  strcat(errMess[0],p2d(second()));
  strcat(errMess[0]," ");
  strcat(errMess[0],mess);
  Serial.println(errMess[0]);
  strcpy(fileName,"/errmess.txt");
  if (openFile("a")) {
    fh.println(errMess[0]);
    fh.close();
  }
}

void clockDisplay() {
  // digital display of the time
  Serial.println();
  Serial.print(year());
  Serial.print("/");
  Serial.print(p2d(month()));
  Serial.print("/");
  Serial.print(p2d(day()));
  Serial.print(" - ");
  Serial.print(p2d(hour()));
  Serial.print(":");
  Serial.print(p2d(minute()));
  Serial.print(":");
  Serial.print(p2d(second()));
  Serial.println();
}

// convert integer into a 2 char string dd
char* p2d(byte b) {
  d2Str[0]=b/10+'0';
  d2Str[1]=b%10+'0';
  return d2Str;
}

// convert float into an 8 char string ddddd.dd
char* p8d(float f) {
  int w = (int)f;
  int d = 10000;
  byte ptr = 0;
  bool started = false;
  for ( int n=0;n<5;n++ ) {
    if ( w/d != 0 || n==4 ) started = true;
    if (started) d8Str[ptr++] = w/d +'0';
    w = w%d;
    d /= 10;
  }
  d8Str[ptr++] = '.';
  d = (int)(100.0*f)-100*(int)f;
  d8Str[ptr++]=d/10 +'0';
  d8Str[ptr++]=d%10 +'0';
  d8Str[ptr]='\0';
  return d8Str;
}
