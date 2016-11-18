#include <Arduino.h>

//  Update power data and title strings for 24 hour graph

void add24PowerData() {
  int outPtr = 0;
  int sampSize = 1440/(numCols-1);
  // start at midnight due to timeofday limitations
  float hi,av,lo;
  for ( int col=0;col<numCols-1;col++ ) {
    addCstring("[[");
    addCstring(p2d(outPtr/60));
    addCstring(",");
    addCstring(p2d(outPtr%60));
    addCstring(",00],");
    hi = 0.0;
    av = 0.0;
    lo = 99.0;
    for ( int s=0;s<sampSize;s++ ) {
      if ( hi < minData[outPtr].hi ) hi = minData[outPtr].hi;
      av += minData[outPtr].av;
      if ( lo > minData[outPtr].hi ) lo = minData[outPtr].lo;
      if (++outPtr>1439) break;
    }
    addCstring(p8d(hi));
    addCstring(",");
    addCstring(p8d(av/sampSize));
    addCstring(",");
    addCstring(p8d(lo));
    addCstring("],");
    }
    // add indicator for current time
    addCstring("[[");
    addCstring(p2d(hour()));
    addCstring(",");
    addCstring(p2d(minute()));
    addCstring(",00],0.0,0.0,0.0],");
    addCstring("[[");
    addCstring(p2d(hour()));
    addCstring(",");
    addCstring(p2d(minute()));
    addCstring(",00],5.0,5.0,5.0]");
}

void add24TitleData() {
  addCstring(p2d(hour()));
  addCstring(":");
  addCstring(p2d(minute()));
  addCstring(":");
  addCstring(p2d(second()));
  addCstring(" / Energy:");
  addCstring(p8d(totalEnergy));
  addCstring(" / Signal:");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring(" / Memory:");
  addCstring(p8d((float)system_get_free_heap_size()/1000.0));
  addCstring(" / Flash:");
  addCstring(p8d((float)fs_info.usedBytes/1000000.0));

}
