/*-------- display code ----------*/

void addCstring(const char* s) {
  // reinventing cstring cat function
  int p;
  for (p=0;p<longStrSize;p++) {
    if ( p>longStrSize-32) {
      fd.print(timeStamp());
      fd.print(p);
      fd.print("- HTML overflow: ");
      fd.println(s);
      break;
    }
    if (longStr[p]=='\0') {
      break;
    }
  }
  int q=0;
  for (;p<longStrSize;p++) {
    longStr[p]=s[q];
    if (s[q++]=='\0') break;
  }
  htmlLen = p;
}

void errMessage(char* mess) {
  Serial.print(timeStamp());
  Serial.println(mess);
  fe.print(dateStamp());
  fe.print(" ");
  fe.print(timeStamp());
  fe.println(mess);
}

void diagMess(char* mess) {
  Serial.print(timeStamp());
  Serial.println(mess);
  fd.print(dateStamp());
  fd.print(" ");
  fd.print(timeStamp());
  fd.println(mess);
}

char* dateStamp() {
  // string with the date
  strcpy(dateStr,p2d(year()%100));
  strcat(dateStr,p2d(month()));
  strcat(dateStr,p2d(day()));
  return dateStr;
}

char* timeStamp() {
  // string with the time
  strcpy(timeStr,p2d(hour()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(minute()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(second()));
  strcat(timeStr," ");
  return timeStr;
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
