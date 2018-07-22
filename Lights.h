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

#ifndef _LIGHTS_H
#define _LIGHTS_H

class Lights {
  public:
    Lights( int pin );
    void update();
    void setTurnState(bool);
    void setBrakeState(bool);
    void setEmergencyState(bool);
    void bright();
    void dim();
    void off();
  private:
    int   pinNum;
    bool  brakeState = 0;
    bool  turnState = 0;
    bool  emergencyState = 0;
    bool  blinkState = 0;
};

class LightsManager {
  public:
    static void init();
    static void update();
    static void process(char *);
    static void leftTurn(bool);
    static void rightTurn(bool);
    static void brakeLights(bool);
    static void emergencyLights(bool);
};

#endif
