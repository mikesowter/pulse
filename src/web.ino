#include <Arduino.h>

char htmlStr0[] =
"<!DOCTYPE html>"
"<html>"
  "<head>"
    "<!--Load the AJAX API-->"
    "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>"
  	"<script type='text/javascript'> google.charts.load('current', {'packages':['corechart']}); google.charts.setOnLoadCallback(drawChart);"
    " function drawChart() {"
        "var data = new google.visualization.DataTable();"
        "data.addColumn(";
char htmlStr1[] =
        ", 'Time');"
        "data.addColumn('number', 'High');"
        "data.addColumn('number', 'Average');"
        "data.addColumn('number', 'Low');"
        "data.addRows([";
char htmlStr2[] =
        "]);"
        "var options = {'title':'";
char htmlStr3[] =
        "             ','width':1000,"
        "              'height':700,"
        "                colors: ['red', 'orange', 'green']};"
        "var chart = new google.visualization.LineChart(document.getElementById('chart_div'));"
        "chart.draw(data, options);"
        "}"
      "</script>"
    "</head>"
  "<body>"
    "<div id='chart_div'></div>"
    "<HR>"
    "Recent messages:";
char htmlStr4[] =
    "<HR>"
  "</body>"
"</html>";

void handleRoot() {
  htmlStr[0]='\0';
  addCstring( htmlStr0 );
  addCstring( "'timeofday'" );
  addCstring( htmlStr1 );
  add3PowerData();
  addCstring( htmlStr2 );
  add3TitleData();
  addCstring( htmlStr3 );
  addErrMess();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleDay() {
  htmlStr[0]='\0';
  addCstring( htmlStr0 );
  addCstring( "'timeofday'" );
  addCstring( htmlStr1 );
  add24PowerData();
  addCstring( htmlStr2 );
  add24TitleData();
  addCstring( htmlStr3 );
  addErrMess();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleAvg() {
  htmlStr[0]='\0';
  addCstring("<!DOCTYPE html><html><body><HR>");
  addAvgData();
  addCstring( htmlStr4 );
  server.send ( 200, "text/html", htmlStr );
  //Serial.println(htmlStr);
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMessage("User requested restart");
    ESP.restart();
  }
  else if (strncmp(userText,"/shutdown",9)==0) {
    errMessage("User requested shutdown");
    uploadDay();
    uploadMonth();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Safe to Shutdown<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Sending File: \"");
    strcat(outBuf,userText);
    strcat(outBuf,"\"<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    strcpy(fileName,userText);
    uploadFile();
    delay(5);
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/dir",4)==0) {
    listFiles();
  }
  else {
    strcpy(outBuf,userText);
    strcat(outBuf," does not exist");
    errMessage(outBuf);
    handleRoot();
  }
}
