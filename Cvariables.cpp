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
#include "config.h"
#include "WifiCommand.h"
#include "Cvariables.h"

int  _CVs[100];
//String _CVstrgs[100];

void Cvariable::initCVs() {
  int i;

  for( i=0; i<100; i++ ) {
    _CVs[i] = -1;
//    _CVstrgs[i] = "";
  }
  _CVs[1] = _TRAIN_NUM; //  Primary Address (train number and appended to host name)
  _CVs[2] = 0;  // Vstart
  _CVs[3] = _SPEED_CHANGE_RATE; // Acceleration Rate
  _CVs[4] =  5; // Deceleration Rate
  _CVs[11] = 0; //   Timeout value 
  //_CVstrgs[46] = _SSID; //     SSID
  //_CVstrgs[47] = _PASSWORD; //     Password
  _CVs[48] = _ETHERNET_PORT; //     Ethernet Port
  //_CVstrgs[49] = _PREFIX; //     Sensor Prefix
  //_CVstrgs[50] = "DCCpp32"; //     Sensor Server names
  _CVs[51] = 1; //     Enable stop on IR detect
  _CVs[52] = 1; //     Enable brake lights on reduced speed or stop
  _CVs[53] = 0; //     Reverse Left/Right turn signals
  _CVs[54] = 768; //     Dim level
  _CVs[55] = 0; //     Bright level
  _CVs[60] = _SCAN_INTERVAL; //     Scan interval in mS
  _CVs[61] = _RECONNECT_INTERVAL; //     Reconnect interval for scan servers
  _CVs[62] = _MAX_RETRIES; //     Max retries
  _CVs[63] = _SENSOR_CHANNEL; //     Sensor Channel  
}

void Cvariable::setCV( int cv, int val ) {
  if( (cv > 0) && (cv < 100) )
    _CVs[cv] = val;
}

int Cvariable::getCV( int cv ) {
  if( (cv > 0) && (cv < 100) )
    return _CVs[cv];
  else
    return -1;
}

void Cvariable::readCV( char *cmd ) {
  int bValue, cv, callBack, callBackSub;

  if(sscanf(cmd,"%d %d %d",&cv,&callBack,&callBackSub)!=3)
    return;    
  
  bValue=getCV( cv );
 
  WiFiCommand::print("<r");
  WiFiCommand::print(callBack);
  WiFiCommand::print("|");
  WiFiCommand::print(callBackSub);
  WiFiCommand::print("|");
  WiFiCommand::print(cv);
  WiFiCommand::print(" ");
  WiFiCommand::print(bValue);
  WiFiCommand::print(">");
}

void Cvariable::writeCV( char *cmd ) {

  int cab, cv, bValue, callBack, callBackSub;

  if(sscanf(cmd,"%d %d %d %d",&cv,&bValue,&callBack,&callBackSub)!=4) {
    if(sscanf(cmd,"%d %d %d",&cab,&cv,&bValue)!=3)
      return;
  }
  else
      cab = 0;  // program on program track
  
  setCV( cv, bValue );
  
  if( !cab ) {
    WiFiCommand::print("<r");
    WiFiCommand::print(callBack);
    WiFiCommand::print("|");
    WiFiCommand::print(callBackSub);
    WiFiCommand::print("|");
    WiFiCommand::print(cv);
    WiFiCommand::print(" ");
    WiFiCommand::print(bValue);
    WiFiCommand::print(">");
  }

}

void Cvariable::writeb( char *cmd ) {
  
}



