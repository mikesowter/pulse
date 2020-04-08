#include <arduino.h>
#include <TimeLib.h>
#include <fs.h>

char date30Back[] = "/XXyymmdd.csv";
extern char fileName[];
extern char charBuf[];

char* i2sd(byte b);
void errMess(const char* mess);

void delOldFiles() {
  // delete all files more than 30 days old
  long SECS_30_DAYS = 30*24*60*60;
  strcpy(date30Back,"/PR");
  strcat(date30Back,i2sd(year(now()-SECS_30_DAYS)%100));
  strcat(date30Back,i2sd(month(now()-SECS_30_DAYS)));
  strcat(date30Back,i2sd(day(now()-SECS_30_DAYS)));
  strcat(date30Back,".csv");

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    dir.fileName().toCharArray(fileName, 14);
    Serial.print(fileName);
    Serial.print("\t");
  //    checkFileSent();
    if (strcmp(fileName,date30Back)<0 && fileName[1]=='P') {
      Serial.print("Remove old files:\n");
      if (SPIFFS.remove(fileName)) {
        strcpy(charBuf,fileName);
        strcat(charBuf," deleted");
        errMess(charBuf);
      }
    }
    else {
      Serial.println(dir.fileSize());
    }
  }
}
