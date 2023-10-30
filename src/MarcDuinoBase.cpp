
#include "MarcDuinoBase.h"

MarcDuinoBase::MarcDuinoBase(VarSpeedServo& Servo1, VarSpeedServo& Servo2, VarSpeedServo& Servo3, VarSpeedServo& Servo4, VarSpeedServo& Servo5, 
                             VarSpeedServo& Servo6, VarSpeedServo& Servo7, VarSpeedServo& Servo8, VarSpeedServo& Servo9, VarSpeedServo& Servo10, VarSpeedServo& Servo11) :
    Servo1(Servo1), Servo2(Servo2), Servo3(Servo3), Servo4(Servo4), Servo5(Servo5), 
    Servo6(Servo6), Servo7(Servo7), Servo8(Servo8), Servo9(Servo9), Servo10(Servo10), Servo11(Servo11) 
{
    memset(SerialBuffer, 0x00, SERIALBUFFERSIZE);
}

void MarcDuinoBase::init()
{
    // I2C
    // Wire.begin();

    // HeartBeat-LED
    pinMode(P_LED2, OUTPUT);
    digitalWrite(P_LED2, HeartBeatStatus);

    HeartBeatMillis = millis();

    memset(SerialBuffer, 0x00, SERIALBUFFERSIZE);
}

void MarcDuinoBase::run()
{
    // Read Serial
    if (Serial.available())
    {
        unsigned char c = Serial.read();
        SerialBuffer[BufferIndex++] = c;
        if ((c == '\r') || (BufferIndex == SERIALBUFFERSIZE))   // Command complete or buffer full
        {
            SerialBuffer[BufferIndex-1] = 0x00; // ensure proper termination
            parseCommand(SerialBuffer);
            memset(SerialBuffer, 0x00, SERIALBUFFERSIZE);
            BufferIndex = 0;
        }
    }

    // Heartbeat
    if ((millis() - HeartBeatMillis) > HEARTBEAT_MILLIS)
    {
        toggleHeartBeat();
        HeartBeatMillis = millis();
    }
}

void MarcDuinoBase::toggleHeartBeat()
{
    #ifdef DEBUG
    // Serial.print(".");
    #endif

    if (HeartBeatStatus == LOW)
      HeartBeatStatus = HIGH;
    else
      HeartBeatStatus = LOW;

    digitalWrite(P_LED2, HeartBeatStatus);
}

bool MarcDuinoBase::separateCommand(const char* command, char* cmd, unsigned int & param_num)
{
    char param[3];
    memset(param, 0x00, 3);

    if (strlen(command) != 5)
    {
        Serial.print("Invalid Size: ");
        Serial.println(strlen(command));
        return false;
    }
    
    memcpy(cmd, command+1, 2);
    memcpy(param, command+3, 2);

    #ifdef DEBUG
    Serial.print("Cmd:   ");
    Serial.println(cmd);
    Serial.print("Param: ");
    Serial.println(param);
    #endif

    param_num = atoi(param);    

    return true;
}

/*
 *	Setup Commands
 * * 
 *	//// SERVO CONTROLS
 *	#SD00 Set Servo direction forward
 *	#SD01 Set servo direction reversed
 *
 *  // OLD, unsupported:
 *	#SRxxy Set individual servo to either forward or reversed xx=servo number y=direction
 *		Must be a 2 digit Servo number i.e. Servo 4 is 04
 *		Must be either 0 or 1 to set the direction (0 normal, 1 reversed)
 *		Use SDxx to globally set the Servo direction, then SRxxy to change individual servos.
 *  // NEW:
 *	#SRxx Set individual servo to reversed xx=servo number
 *		Must be a 2 digit Servo number i.e. Servo 4 is 04
 *		Use SDxx to globally set the Servo direction, then SRxxy to change individual servos.
 *	#SNxx Set individual servo to normal xx=servo number
 *		Must be a 2 digit Servo number i.e. Servo 4 is 04
 *		Use SDxx to globally set the Servo direction, then SRxxy to change individual servos.
 *
 *  #SOxxddd Set Servo Degrees for Panel Open,  ddd=000-180 (90deg, Servo1: SO01090)
 *  #SCxxddd Set Servo Degrees for Panel Closed ddd=000-180 (180deg, Servo1: SO01180)
 *  #SPxxddd Set Servo Speed, ddd=0-255
 * 
 * *	//// STARTUP SOUND CONTROLS
 *	#SS00 Disable Startup Sound, and remove startup sound delay for fast boot of R2
 *	#SS01 Default Startup Sound in file 255
 *	#SS02 Alternate Startup Sound in file 254
 *	#SS03 Second Alternate Startup Sound in file 253
 *
 *	// Chatty/Silent Mode
 *	#SQ00 Default Chatty Mode
 *	#SQ01 Silent on startup
 *
 *	//// PANEL SEQUENCER CONTROLS
 *	#STxx Setup Delay time between Master and Slave Panel Sequences.
 *		Use this if the Slave panels are starting too soon
 *		Values up to 250 are supported.  Values are in ms.
 *
 * //// MAIN CONFIG
 *  #MDxx Set MarcDuino Mode
 *      #MD00 : MarcDuino Dome Master
 *      #MD01 : MarcDuino Dome Slave
 *      #MD02 : MarcDuino Body Master
 *      MarcDuino will reboot immediately after setup and start in new mode.
 * 
 * //// SYSTEM FUNCTIONS
 *  #DMxx Dump EEPROM at address xx
 *  #RSET Reboot MarcDuino
 */

void MarcDuinoBase::processSetupCommand(const char* command)
{
    char cmd[3];
    unsigned int param_num = 0;

    memset(cmd, 0x00, 3);

    #ifdef DEBUG
    Serial.print("SetupCommand(Base): ");
    Serial.println((const char*)command);
    #endif

    if (!separateCommand(command, cmd, param_num))
        return;

    if (strcmp(cmd, "SD") == 0)             // Servo Direction
    {
    }
    else if (strcmp(cmd, "SR") == 0)       // Individual Servo Reverse
    {
    }
    else if (strcmp(cmd, "SN") == 0)       // Individual Servo Normal
    {
    }
    else if (strcmp(cmd, "SS") == 0)       // Sound Control
    {
    }
    else if (strcmp(cmd, "SQ") == 0)       // Chatty Mode
    {
    }
    else if (strcmp(cmd, "ST") == 0)       // Delay Time Master/Slave
    {
    }
    else if (strcmp(cmd, "MD") == 0)        // Set MarcDuino Type and reboot in new mode
    {
        switch(param_num)
        {
            case 0:
                Storage.setType(MarcDuinoStorage::DomeMaster);
                delay(500);
                resetFunc();
            break;
            case 1:
                Storage.setType(MarcDuinoStorage::DomeSlave);
                delay(500);
                resetFunc();
            break;
            case 2:
                Storage.setType(MarcDuinoStorage::BodyMaster);
                delay(500);
                resetFunc();
            break;
            default:
            break;
        }
    }
    else if (strcmp(cmd, "DM") == 0)             // Dump EEPROM
    {
        Storage.dumpToSerial();
    }
    else if (strcmp(cmd, "RS") == 0)             // Reboot MarcDuino
    {
        delay(500);
        resetFunc();
    }
}
