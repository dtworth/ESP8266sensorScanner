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
#include "Motor.h"

int _targetSpeed = 0 ;
int _prevSpeed = 0 ;
int _resumeSpeed = 0;
long _lastCheckTime = 0L;
bool _motorStopped = true;

void Motor::init(){
}

void Motor::update(){

  if( _motorStopped )
    return;
  int speedIncrement = _SPEED_CHANGE_RATE * (millis() - _lastCheckTime) / 1000;
  if( speedIncrement ) {
    if( _prevSpeed < _targetSpeed ) {
      _prevSpeed += speedIncrement;
      if( _prevSpeed > _targetSpeed )
        _prevSpeed = _targetSpeed;
    }
    else if ( _prevSpeed > _targetSpeed ) {
      if( _targetSpeed == 0 )   // brake applied
        speedIncrement *= 2;
      _prevSpeed -= speedIncrement;
      if( _prevSpeed < _targetSpeed )
        _prevSpeed = _targetSpeed;
    }
    _lastCheckTime = millis();
    analogWrite( 1, 1024 - (int)(1024.0 * _prevSpeed / 100.0) );
  }
}

void Motor::setSpeed(int speed){
  _targetSpeed = speed;
  _lastCheckTime = millis();
}

void Motor::setStop(){
  if( !_motorStopped )
    _resumeSpeed = _targetSpeed;
  _motorStopped = true;
  setSpeed(0);
}

void Motor::setEstop(){
  setStop();
  _prevSpeed = 0;
  analogWrite( 1, 1024 );
}

void Motor::setResume(){
  if( _motorStopped ) {
    setSpeed( _resumeSpeed );
    _motorStopped = false;
  }
}

