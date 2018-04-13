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

String  serverNames[] = { _SERVER_NAMES };
DeliverResult place[ sizeof( serverNames) ];

void DeliverResult::init( String serverName ) {
  _serverName = serverName;
}

void DeliverResult::sendString( int blockNum, int trainNum ) {
  _client.print( "<RS " );
  _client.print( blockNum );
  _client.print( " " );
  _client.print( trainNum );
  _client.print( ">" );
}

void DeliverResult::send( int blockNum ) {
    if( !_client.connected() )
      _client.connect( _serverName, 2560 );
    if( _client.connected() ) {
      Serial.println( blockNum );
      if( (blockNum != _prevBlock) && (_prevBlock > 0) )
        sendString( _prevBlock, 0 );
      if( blockNum > 0 )
        sendString( blockNum, _TRAIN_NUM );
      _prevBlock = blockNum;
      _client.stop();
    }
}


void DeliverResultsManager::init() {
  place[0].init( serverNames[0] );
}

void DeliverResultsManager::send( int blockNum ) {
  place[0].send( blockNum );
}

