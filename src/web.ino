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
  Serial.println(htmlStr);
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
  if (strncmp(userText,"/reset",5)==0) {
    Serial.println("User requested restart");
    ESP.restart();
  }
  
	/* String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
  Serial.println(message);

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }  */

}
