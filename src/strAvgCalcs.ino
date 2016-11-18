#include <Arduino.h>

// add a text page with average power levels over the previous 1,2,4,8,15,30...  minutes
// too allow independent processors to make load decisions on the cost of power

void addAvgData() {
  int outPtr = 60*hour()+minute();
  float sum=0.0;
  int intervals[9]={1,2,4,8,15,30,60,120,240};
  for ( int min=1;min<=240;min++ ) {
    if (--outPtr < 0) outPtr += 1440;
    sum += minData[outPtr].av;
    for ( int index=0;index<9;index++) {
      if (min==intervals[index]) {
        addCstring("<P>");
        addCstring(p2d(min%100));
        addCstring(": ");
        addCstring(p8d(sum/min));
      }
    }
  }
}
