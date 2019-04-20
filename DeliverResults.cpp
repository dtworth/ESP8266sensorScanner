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
#include "DeliverResults.h"

std::vector<String>  serverNames = { _SERVER_NAMES };
DeliverResult place[ _MAX_SERVER_NAMES  ];

void DeliverResult::init( String serverName ) {
  _serverName = serverName;
}

void DeliverResult::sendString( int blockNum, int trainNum ) {
  char buffer[80];

  sprintf( buffer, "<RS %i %i>", blockNum, trainNum );
  _client.print( buffer );
}

void DeliverResult::send( int blockNum ) {
    if( blockNum == _newBlock ) { // same block reported twice in a row
      if( !_client.connected() && (millis() > _lastConnectionTime + _RECONNECT_INTERVAL) && ( _retryCount < _MAX_RETRIES ) ) {
        _client.connect( _serverName, 2560 );
        Serial.print( "Connect to " );
        Serial.println( _serverName );
        _lastConnectionTime = millis();   // update whether it connects or not
        _retryCount++;
      }
      if( _client.connected() ) {
        Serial.print( "Connected to " );
        Serial.println( _serverName );
        _lastConnectionTime = 0;          // insure that next connection occurs quickly
        _retryCount = 0;
        if( (blockNum != _prevBlock) && (_prevBlock > 0) )
          sendString( _prevBlock, 0 );
        if( blockNum > 0 ) {
          sendString( blockNum, _TRAIN_NUM );
          Serial.print( "Send String to " );
          Serial.println( _serverName );
        }
        _prevBlock = blockNum;
        delay(100);
        _client.stop();
      }
    }
    else
      _newBlock = blockNum;
}


void DeliverResultsManager::init() {
  for( int i=0; i<serverNames.size(); i++ )
    place[i].init( serverNames[i] );
}

void DeliverResultsManager::send( int blockNum ) {
  for( int i=0; i<serverNames.size(); i++ )
    place[i].send( blockNum );
}

