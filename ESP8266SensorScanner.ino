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
#include "WifiCommand.h"
#include "DeliverResults.h"
#include "Lights.h"
#include "Motor.h"

ESP8266WebServer webServer(80);

ADC_MODE(ADC_VCC);      // set A0 to read VCC

void handleRoot()
{
  char buff[20];
  int speed=0;
  bool left=0, right=0, brake=0, emergency=0;

  LightsManager::leftTurn(0);
  LightsManager::rightTurn(0);
  LightsManager::emergencyLights(0);
  LightsManager::brakeLights(0);

  for(int i=0;i<webServer.args();i++) {
    if( !strcmp( webServer.argName(i).c_str(), "speed" ) ) {
      speed = webServer.arg(i).toInt();
      Motor::setSpeed( speed );
    }
    else if( !strcmp( webServer.argName(i).c_str(), "left" ) ) {
      left = 1;
      LightsManager::leftTurn(1);
    }
    else if( !strcmp( webServer.argName(i).c_str(), "right" ) ) {
      right = 1;
      LightsManager::rightTurn(1);
    }
    else if( !strcmp( webServer.argName(i).c_str(), "emergency" ) ) {
      emergency = 1;
      LightsManager::emergencyLights(1);
    }
    else if( !strcmp( webServer.argName(i).c_str(), "brake" ) ) {
      brake = 1;
      LightsManager::brakeLights(1);
    }
  }

  int vccVoltage = ESP.getVcc();

  String message = "<html><body>\n"; 
  message += "<h1>Car 01</h1>\n"; 
  message += "<form method='GET'>\n";

  message += "<p>Vcc=";  
  message += itoa( vccVoltage, buff, 10 );
  message += "</p>";  

  message += "<p>Left Turn? <input type='checkbox' ";
  if( left )
    message += "checked ";
  message += " name='left'/></p>";

  message += "<p>Right Turn? <input type='checkbox' ";
  if( right )
    message += "checked ";
  message += " name='right'/></p>";

  message += "<p>Emergency Lights? <input type='checkbox' ";
  if( emergency )
    message += "checked ";
  message += " name='emergency'/></p>";

  message += "<p>Brake Lights? <input type='checkbox' ";
  if( brake )
    message += "checked ";
  message += " name='brake'/></p>";

  message += "<p>D3(Rx)=";  
  message += digitalRead(3) ? "1": "0";  
  message += "</p>";  
  
  message += "<p>Speed (0-100): <input type='range' min='0' max='100' step='5' value='";
  message += itoa( speed, buff, 10 );
  message += "' name='speed'/></p>";
  message += "<input type='submit' value='Update'/>";
  message += "</form></body></html>\n";
  webServer.send(200, "text/html", message);
}


void setup() {
  webServer.on( "/", handleRoot );
  webServer.begin();

  ScanSensors::init();
  WiFiCommand::init();
  DeliverResultsManager::init();
  LightsManager::init();
  Motor::init();
}


void loop() {   // scan for strongest signal and send to server
  char c;
  int numNetworks, blockNum;

  if( (blockNum = ScanSensors::scan()) >= 0 ) {
    DeliverResultsManager::send( blockNum );
  }
  webServer.handleClient();
  WiFiCommand::process();   // check for, and process, and new WiFi commands
  if( digitalRead(3) )
    Motor::setResume();
  else
    Motor::setEstop();
  Motor::update();
}
