/*
MIT License

Copyright (c) 2018 Dan Worth
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "ScanSensors.h"
#include "DeliverResults.h"

ESP8266WebServer server(80);
ADC_MODE(ADC_VCC);      // set A0 to read VCC

void handleRoot()
{
  char buff[20];

  int vccVoltage = ESP.getVcc();
  int speed = server.arg(0).toInt();
  if( speed < 0 )
    speed = 0;
  else if( speed > 1023 )
    speed = 1023;
  analogWrite(1, 1023 - speed);    // use the Tx pin for output
  String message = "<html><body>\n"; 
  message += "<h1>Car 01</h1>\n"; 
  message += "<form method='GET'>\n";
  message += "<p>Vcc=";  
  message += itoa( vccVoltage, buff, 10 );
  message += "</p>";  
  message += "<p>D0=";  
  message += digitalRead(0) ? "1": "0";  
  message += "</p>";  
  message += "<p>D2=";  
  message += digitalRead(2) ? "1": "0";  
  message += "</p>";
  message += "<p>D3(Rx)=";  
  message += digitalRead(3) ? "1": "0";  
  message += "</p>";  
  message += "<p>Speed (0-1023): <input type='text' value='";
  message += itoa( speed, buff, 10 );
  message += "' name='speed'/></p>";
  message += "<input type='submit' value='Update'/>";
  message += "</form></body></html>\n";
  server.send(200, "text/html", message);
}


void setup() {
  server.on( "/", handleRoot );
  server.begin();

  ScanSensors::init();
  DeliverResultsManager::init();
}


void loop() {   // scan for strongest signal and send to server
  char c;
  int numNetworks, blockNum;

  if( (blockNum = ScanSensors::scan()) >= 0 ) {
    DeliverResultsManager::send( blockNum );
  }
  server.handleClient();
}
