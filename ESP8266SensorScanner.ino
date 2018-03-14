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

#include <ESP8266WiFi.h>

#define PREFIX "sensor"             // the prefix for the sensor Acess Points
const char* ssid = "";              // the ssid of your wifi router
const char* password = "";          // the password for above
const char* serverName = "DCCpp32"; // the DCC++32 where status updates are delivered
String host = "train01";
const int trainNum = 1;
int prevBlock = 0;
int maxBlock = -1;

WiFiClient  client;

int measureSignals(int NetworksFound)
{
  char  Buffer[80];
  int   block, level, maxLevel = -127;

  for (int i = 0; i < NetworksFound; i++) {
    strcpy( Buffer, WiFi.SSID(i).c_str() );
    if ( !strncmp( PREFIX, Buffer, strlen( PREFIX ) ) ) {
      block = atoi( &Buffer[strlen(PREFIX)] );
      level = WiFi.RSSI(i);
      if ( level > maxLevel ) {
        maxBlock = block;
        maxLevel = level;
      }
    }
  }
  return maxBlock;
}

void connectToWiFi()
{
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.hostname( host );
  WiFi.begin(ssid, password);
  WiFi.hostname( host );
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.hostname());
}

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

  connectToWiFi();
}


void loop() {   // scan for strongest signal and send to server
  char c;
  int numNetworks, blockNum;

  if ( (numNetworks = WiFi.scanComplete()) == -2 )
    WiFi.scanNetworks(true);
  else if (numNetworks >= 0) {
    client.connect( serverName, 2560 );
    blockNum = measureSignals( numNetworks );
    WiFi.scanDelete();
    if( (blockNum > 0) && client.connected() ) {
      Serial.println( blockNum );
      if( (blockNum != prevBlock) && prevBlock )
        sendString( prevBlock, 0 );
      sendString( blockNum, trainNum );
      prevBlock = blockNum;
      client.stop();
    }
  }
}
