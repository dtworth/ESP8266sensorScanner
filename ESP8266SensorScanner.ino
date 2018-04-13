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
#include "ScanSensors.h"

int prevBlock = -1;

WiFiClient  client;

void sendString( int blockNum, int trainNum ) {
  client.print( "<RS " );
  client.print( blockNum );
  client.print( " " );
  client.print( trainNum );
  client.print( ">" );
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  ScanSensors::init();
}


void loop() {   // scan for strongest signal and send to server
  char c;
  int numNetworks, blockNum;

  if( (blockNum = ScanSensors::scan()) >= 0 ) {
    if( !client.connected() )
      client.connect( _SERVER_NAME, 2560 );
    if( client.connected() ) {
      Serial.println( blockNum );
      if( (blockNum != prevBlock) && (prevBlock > 0) )
        sendString( prevBlock, 0 );
      if( blockNum > 0 )
        sendString( blockNum, _TRAIN_NUM );
      prevBlock = blockNum;
      client.stop();
    }
  }
}
