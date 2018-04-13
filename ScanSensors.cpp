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

long lastScanTime = 0;

void ScanSensors::init() {
  Serial.print("\n\nConnecting to ");
  Serial.println(_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.hostname( _HOST );
  WiFi.begin(_SSID, _PASSWORD);
  WiFi.hostname( _HOST );
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.hostname());

}

int ScanSensors::scan() {
  int numNetworks, blockNum = -1;

  if ( (numNetworks = WiFi.scanComplete()) == -2 ) {
    if ( millis() > lastScanTime + _SCAN_INTERVAL ) {
      lastScanTime = millis();
      WiFi.scanNetworks(true, false, _SENSOR_CHANNEL );
    }
  }
  else if (numNetworks >= 0) {
    blockNum = measureSignals( numNetworks );
    WiFi.scanDelete();
  }
  return blockNum;
}


int ScanSensors::measureSignals(int NetworksFound)
{
  char  Buffer[80];
  int   block, level, maxLevel = -127;
  int   maxBlock = -1;

  for (int i = 0; i < NetworksFound; i++) {
    strcpy( Buffer, WiFi.SSID(i).c_str() );
    if ( !strncmp( _PREFIX, Buffer, strlen( _PREFIX ) ) ) {
      block = atoi( &Buffer[strlen(_PREFIX)] );
      level = WiFi.RSSI(i);
      if ( level > maxLevel ) {
        maxBlock = block;
        maxLevel = level;
      }
    }
  }
  return maxBlock;
}
