/**********************************************************************

WiFiCommand.cpp
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

// DCC++ BASE STATION COMMUNICATES VIA WIFI USING SINGLE-CHARACTER TEXT COMMANDS
// WITH OPTIONAL PARAMTERS, AND BRACKETED BY < AND > SYMBOLS.  SPACES BETWEEN PARAMETERS
// ARE REQUIRED.  SPACES ANYWHERE ELSE ARE IGNORED.  A SPACE BETWEEN THE SINGLE-CHARACTER
// COMMAND AND THE FIRST PARAMETER IS ALSO NOT REQUIRED.

// See WiFiCommand::parse() below for defined text commands.

#include <ESP8266WiFi.h>
#include "config.h"
#include "WiFiCommand.h"
#include "Motor.h"
#include "Lights.h"
//#include "Sensor.h"
//#include "RemoteSensor.h"
//#include "EEStore.h"

WiFiServer   telnetServer(_ETHERNET_PORT);
WiFiClient   client[MAX_CLIENTS];

///////////////////////////////////////////////////////////////////////////////

char WiFiCommand::commandString[MAX_CLIENTS][MAX_COMMAND_LENGTH+1];

///////////////////////////////////////////////////////////////////////////////

void WiFiCommand::init(){
  telnetServer.begin();
}

void WiFiCommand::process(){
  char c;

  if (telnetServer.hasClient()) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!client[i] || !client[i].connected()) {
        if (client[i]) {
          client[i].stop();
        }
        client[i] = telnetServer.available();
        continue;
      }
    }
    telnetServer.available().stop();
  }
  for (int i = 0; i < MAX_CLIENTS; i++)
    if (client[i] && client[i].connected()) {
      if( client[i].connected() ) {
        if( client[i].available()){        // while there is data on the network
          c=client[i].read();
          if(c=='<')                    // start of new command
            sprintf(commandString[i],"");
          else if(c=='>') {              // end of new command
            parse(commandString[i]);
          }
          else if(strlen(commandString[i])<MAX_COMMAND_LENGTH)    // if comandString still has space, append character just read from network
            sprintf(commandString[i],"%s%c",commandString[i],c);     // otherwise, character is ignored (but continue to look for '<' or '>')
        }
      } // while
    }
} // WiFiCommand:process
   
///////////////////////////////////////////////////////////////////////////////

void WiFiCommand::parse(char *com){
  
  switch(com[0]){
/***** SET ENGINE THROTTLES USING 128-STEP SPEED CONTROL ****/    
        case 't':       // <t REGISTER CAB SPEED DIRECTION>
/*
 *    sets the throttle for a given register/cab combination 
 *    
 *    REGISTER: an internal register number, from 1 through MAX_MAIN_REGISTERS (inclusive), to store the DCC packet used to control this throttle setting
 *              (this is variable is ignored, but must be present)
 *    CAB:  the short (1-127) or long (128-10293) address of the engine decoder
 *              (this is variable is ignored, but must be present)
 *    SPEED: throttle speed from 0-126, or -1 for emergency stop (resets SPEED to 0)
 *    DIRECTION: 1=forward, 0=reverse.  Setting direction when speed=0 or speed=-1 only effects directionality of cab lighting for a stopped train
 *              (this is variable is ignored, but must be present.  Direction is always forward)
 *    
 *    returns: <T REGISTER SPEED DIRECTION>
 *    
 */
      Motor::process(com+1);
      break;
/***** OPERATE ENGINE DECODER FUNCTIONS F0-F28 ****/    

    case 'f':       // <f CAB BYTE1 [BYTE2]>
/*
 *    turns on and off engine decoder functions F0-F28 (F0 is sometimes called FL)  
 *    NOTE: setting requests transmitted directly to mobile engine decoder --- current state of engine functions is not stored by this program
 *    
 *    CAB:  the short (1-127) or long (128-10293) address of the engine decoder
 *              (this is variable is ignored, but must be present)
 *    
 *    To set functions F0-F4 on (=1) or off (=0):
 *      
 *    BYTE1:  128 + F1*1 + F2*2 + F3*4 + F4*8 + F0*16
 *    BYTE2:  omitted
 *   
 *    To set functions F5-F8 on (=1) or off (=0):
 *   
 *    BYTE1:  176 + F5*1 + F6*2 + F7*4 + F8*8
 *    BYTE2:  omitted
 *   
 *    To set functions F9-F12 on (=1) or off (=0):
 *   
 *    BYTE1:  160 + F9*1 +F10*2 + F11*4 + F12*8
 *    BYTE2:  omitted
 *   
 *    To set functions F13-F20 on (=1) or off (=0):
 *   
 *    BYTE1: 222 
 *    BYTE2: F13*1 + F14*2 + F15*4 + F16*8 + F17*16 + F18*32 + F19*64 + F20*128
 *   
 *    To set functions F21-F28 on (=1) of off (=0):
 *   
 *    BYTE1: 223
 *    BYTE2: F21*1 + F22*2 + F23*4 + F24*8 + F25*16 + F26*32 + F27*64 + F28*128
 *   
 *    returns: NONE
 * 
 */
      LightsManager::process(com+1);
      break;



    case 's':      // <s>
/*
 *    returns status messages containing track power status, throttle status, turn-out status, and a version number
 *    NOTE: this is very useful as a first command for an client to send to this sketch in order to verify connectivity and update any GUI to reflect actual throttle and turn-out settings
 *    
 *    returns: series of status messages that can be read by an client to determine status of DCC++ Base Station and important settings
 */
      WiFiCommand::print("<p0>");
      WiFiCommand::print("<Faller Vehicle ESP8266 Controller");
//      WiFiCommand::print(": V-");
//      WiFiCommand::print(VERSION);
      WiFiCommand::print(" / ");
      WiFiCommand::print(__DATE__);
      WiFiCommand::print(" ");
      WiFiCommand::print(__TIME__);
      WiFiCommand::print(">");
      WiFiCommand::print("<N");
      WiFiCommand::print("1");
      WiFiCommand::print(": ");
      WiFiCommand::print(WiFi.localIP().toString().c_str());
      WiFiCommand::print(">");          
                      
      break;

/***** STORE SETTINGS IN EEPROM  ****/    

    case 'E':     // <E>
/*
 *    stores settings for turnouts and sensors EEPROM
 *    
 *    returns: <e nTurnouts nSensors>
*/
     
//    EEStore::store();
//    WiFiCommand::print("<e ");
//    WiFiCommand::print(EEStore::eeStore->data.nTurnouts);
//    WiFiCommand::print(" ");
//    WiFiCommand::print(EEStore::eeStore->data.nSensors);
//    WiFiCommand::print(" ");
//    WiFiCommand::print(EEStore::eeStore->data.nOutputs);
//    WiFiCommand::print(">");
    break;
    
/***** CLEAR SETTINGS IN EEPROM  ****/    

    case 'e':     // <e>
/*
 *    clears settings for Turnouts in EEPROM
 *    
 *    returns: <O>
*/
     
//    EEStore::clear();
//    WiFiCommand::print("<O>");
    break;

/***** PRINT CARRIAGE RETURN IN SERIAL MONITOR WINDOW  ****/    
                
    case ' ':     // < >                
/*
 *    simply prints a carriage return - useful when interacting with Ardiuno through serial monitor window
 *    
 *    returns: a carriage return
*/
      WiFiCommand::print("\r\l");
      break;  
//    case 'R': 
//      if( com[1] == 'S' )
/*   
 *   *** SEE REMOTESENSOR.CPP FOR COMPLETE INFO ON THE DIFFERENT VARIATIONS OF THE "RS" COMMAND
 *   USED TO CREATE/EDIT/REMOVE/SHOW SENSOR DEFINITIONS
 */
//        RemoteSensor::parse(com+2);
      break;
  } // switch
}; // WiFiCommand::parse

///////////////////////////////////////////////////////////////////////////////

void WiFiCommand::print( char * string ) {
  for (int i = 0; i < MAX_CLIENTS; i++)
    if (client[i] && client[i].connected()) {
      client[i].print(string);
      delay(1);
    }
}

  
void WiFiCommand::print(const char * string ) {
  char buffer[30];
  strcpy( buffer, string );
  WiFiCommand::print( buffer );
}

    
void WiFiCommand::print(int  num) {
  char buffer[30];
  itoa( num, buffer, 10 );  
  WiFiCommand::print( buffer );
}

