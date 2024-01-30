#ifndef __MARCDUINOBODYMASTERCHOPPER_H__
#define __MARCDUINOBODYMASTERCHOPPER_H__

#include <SendOnlySoftwareSerial.h>

#include "MDuinoDomeMaster.h"

/** Command implementation
 * 
 *  SAME AS R2D2 DOME MASTER, but different sequences
 *
 *	Sequences details (see sequence_command, panel sequence matrices defined in panel_sequences.h)
 *
 *	:SE30 Utility Arm open and close sequence
 *	:SE31 All Body Panels open and close sequence
 *	:SE32 All Body Doors open and wiggle close sequence
 *	:SE33 Body - Use Gripper
 *	:SE34 Body - Use Interface Tool
 *	:SE35 Body - Ping Pong Body Doors
 
 */

class MDuinoDomeMasterChopper : public MDuinoDomeMaster
{
    public:
        MDuinoDomeMasterChopper(SendOnlySoftwareSerial& Serial_Slave, SendOnlySoftwareSerial& Serial_MP3,
            VarSpeedServo& Servo1, VarSpeedServo& Servo2, VarSpeedServo& Servo3, VarSpeedServo& Servo4, VarSpeedServo& Servo5, 
            VarSpeedServo& Servo6, VarSpeedServo& Servo7, VarSpeedServo& Servo8, VarSpeedServo& Servo9, VarSpeedServo& Servo10, 
            VarSpeedServo& Servo11, VarSpeedServo& Servo12, VarSpeedServo& Servo13);

        virtual void init() override;
        virtual void run() override;

        virtual const char* getProductName() override { return "MarcDuino Dome Master Chopper"; }

    protected:
        void playSequence(const byte SeqNr) override;
        void playSequenceAddons(const byte SeqNr) override;
        
};

#endif  // __MARCDUINOBODYMASTERCHOPPER_H__