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
#include <Ticker.h>
#include "config.h"
#include "Lights.h"

Ticker blinker;         // timer for turn signals
Lights leftLight(2), rightLight(0);

Lights::Lights( int pin ) {
  pinNum = pin;
}

void Lights::update() {
  if( turnState || emergencyState ) {
    if( blinkState )
      if( brakeState )
        dim();
      else
        off();
    else
      if( brakeState )
        bright();
      else
        dim();
    blinkState = !blinkState;
  }
  else {
    if( brakeState )
      bright();
    else
      dim();
    blinkState = 0;    
  }
}

void Lights::setTurnState(bool state) {
  turnState = state;
}

void Lights::setBrakeState(bool state) {
  brakeState = state;
}

void Lights::setEmergencyState(bool state) {
  emergencyState = state;
}

void Lights::bright() {
  analogWrite( pinNum, 0 );
}

void Lights::dim() {
  analogWrite( pinNum, 640 );
}

void Lights::off() {
  analogWrite( pinNum, 1023 );
}

void LightsManager::init(){
  blinker.attach(0.3, LightsManager::update);
}

void LightsManager::update(){
  leftLight.update();
  rightLight.update();
}

void LightsManager::leftTurn(bool state){
  leftLight.setTurnState(state);
  if( state )
    rightLight.setTurnState(0);
}

void LightsManager::rightTurn(bool state){
  if( state )
    leftLight.setTurnState(0);
  rightLight.setTurnState(state);
}

void LightsManager::brakeLights(bool state){
  leftLight.setBrakeState(state);
  rightLight.setBrakeState(state);
}

void LightsManager::emergencyLights(bool state) {
  leftLight.setEmergencyState(state);
  rightLight.setEmergencyState(state);
}


