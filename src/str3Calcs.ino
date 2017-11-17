#include <Arduino.h>

//  Update power data and title strings for 3 hour graph

void add3PowerData() {
  int outPtr = 60*(int)oldHour+(int)oldMin;
  outPtr -= numCols;
  if (outPtr<0) outPtr += 1440;
  // start with oldest values (next cell to be overridden)
  for ( int col=0;col<numCols;col++ ) {
    if ( outPtr == 1440 ) outPtr = 0;
    addCstring("[[");
    addCstring(p2d(outPtr/60));
    addCstring(",");
    addCstring(p2d(outPtr%60));
    addCstring(",00],");
    addCstring(p8d(minData[outPtr].hi));
    addCstring(",");
    addCstring(p8d(minData[outPtr].av));
    addCstring(",");
    addCstring(p8d(minData[outPtr++].lo));
    addCstring("]");
    if (col!=numCols) addCstring(",");
  }
}

void add3TitleData() {
  addCstring(p2d(hour()));
  addCstring(":");
  addCstring(p2d(minute()));
  addCstring(":");
  addCstring(p2d(second()));
  addCstring(" / Energy:");
  addCstring(p8d(T11Energy));
  addCstring(" / Power:");
  addCstring(p8d(power));
  addCstring(" / Signal:");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring(" / Freemem:");
  addCstring(p8d((float)system_get_free_heap_size()));
}

void addErrMess() {
  addCstring("<P>");
  addCstring(errMess[0]);
  addCstring("<P>");
  addCstring(errMess[1]);
  addCstring("<P>");
  addCstring(errMess[2]);
  addCstring("<P>");
  addCstring(errMess[3]);
  addCstring("<P>");
  addCstring(errMess[4]);
}

void addCstring(char* s) {
  // find end of htmlStr
  int p;
  for (p=0;p<HTML_SIZE;p++) {
    if ( p>HTML_SIZE-32) {
      fd.print(timeStamp());
      fd.print(p);
      fd.print("- HTML overflow: ");
      fd.println(s);
      break;
    }
    if (htmlStr[p]=='\0') {
      break;
    }
  }
  int q=0;
  for (;p<HTML_SIZE;p++) {
    htmlStr[p]=s[q];
    if (s[q++]=='\0') break;
  }
  htmlLen = p;
}

float reason(float f) {
  if ( f>20.0) return 20.0;
  if ( f< 0.0) return 0.0;
  return f;
}
