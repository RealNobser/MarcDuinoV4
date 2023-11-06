#include"MarcDuinoBodyMaster.h"
#include "PanelSequences.h"

MarcDuinoBodyMaster::MarcDuinoBodyMaster(SendOnlySoftwareSerial& Serial_Slave, SendOnlySoftwareSerial& Serial_MP3,
            VarSpeedServo& Servo1, VarSpeedServo& Servo2, VarSpeedServo& Servo3, VarSpeedServo& Servo4, VarSpeedServo& Servo5, 
            VarSpeedServo& Servo6, VarSpeedServo& Servo7, VarSpeedServo& Servo8, VarSpeedServo& Servo9, VarSpeedServo& Servo10, 
            VarSpeedServo& Servo11, VarSpeedServo& Servo12, VarSpeedServo& Servo13):
    MarcDuinoDomeMaster(Serial_Slave, Serial_MP3, Servo1, Servo2, Servo3, Servo4, Servo5, Servo6, Servo7, Servo8, Servo9, Servo10, Servo11, Servo12, Servo13)
{

}

void MarcDuinoBodyMaster::init()
{
    MarcDuinoDomeMaster::init();
}

void MarcDuinoBodyMaster::run()
{
    MarcDuinoDomeMaster::run();
}

void MarcDuinoBodyMaster::playSequence(const unsigned int SeqNr)
{
    Sequencer.stopSequence();
    Sequencer.clearSequence();
    
    switch (SeqNr)
    {
    case 0: // CLOSE ALL PANELS
        Sequencer.loadSequence(body_panel_init, SEQ_SIZE(panel_init));
        Sequencer.setServoSpeed(MarcDuinoSequencer::slow);
        break;
    case 1:  // SCREAM
    case 51:
        Sequencer.loadSequence(body_panel_all_open, SEQ_SIZE(panel_all_open));
        Sequencer.setServoSpeed(MarcDuinoSequencer::slow);
        break;
    case 2: // WAVE
    case 52:
        Sequencer.loadSequence(body_panel_wave, SEQ_SIZE(panel_wave));
        Sequencer.setServoSpeed(MarcDuinoSequencer::full);
        break;
    case 6: // SHORT CIRCUIT / FAINT
    case 56:
        Sequencer.loadSequence(body_panel_all_open_long, SEQ_SIZE(panel_all_open_long));
        Sequencer.setServoSpeed(MarcDuinoSequencer::super_slow);
        break;
    case 8: // LEIA
    case 10:
    case 11:
    case 13:
    case 14:
        Sequencer.loadSequence(body_panel_init, SEQ_SIZE(panel_init));	// Close panels
        Sequencer.setServoSpeed(MarcDuinoSequencer::slow);
        break;

    // 
    // 16 - 29 reserved for dome reagrding Eebel
    //

    //
    // Body Sequences
    //
    case 30: // Utility ARM open and close sequence
        Sequencer.loadSequence(body_utility_arms_open, SEQ_SIZE(body_utility_arms_open));
        Sequencer.setServoSpeed(MarcDuinoSequencer::medium);
        break;
    case 31: // All Body Panels open and close sequence
        Sequencer.loadSequence(body_panel_all_test, SEQ_SIZE(body_panel_all_test));
        Sequencer.setServoSpeed(MarcDuinoSequencer::medium);
        break;
    case 32: // All Body Doors open and wiggle close sequence
        Sequencer.loadSequence(body_panel_spook, SEQ_SIZE(body_panel_spook));
        Sequencer.setServoSpeed(MarcDuinoSequencer::full);
        break;
    case 33: //Body - Use Gripper
        Sequencer.loadSequence(body_panel_use_gripper, SEQ_SIZE(body_panel_use_gripper));
        Sequencer.setServoSpeed(MarcDuinoSequencer::full);
        break;
    case 34: //Body - Use Interface Tool
        Sequencer.loadSequence(body_panel_use_interface_tool, SEQ_SIZE(body_panel_use_interface_tool));
        Sequencer.setServoSpeed(MarcDuinoSequencer::full);
        break;
    case 35: //Body - Ping Pong Body Doors
        Sequencer.loadSequence(body_panel_pingpong_Doors, SEQ_SIZE(body_panel_pingpong_Doors));
        Sequencer.setServoSpeed(MarcDuinoSequencer::full);
        break;
    default:
        break;         
    }
    playSequenceAddons(SeqNr);
}

void MarcDuinoBodyMaster::playSequenceAddons(const unsigned int SeqNr)
{
    // Also forward to Slave
    Serial_Slave.printf(F(":SE%2d\r"), SeqNr);

    // Disable Servo detach during Animations
    ServoBuzzIntervall = 0;
    Sequencer.addSequenceCompletionCallback(sequenceCallbackBuzz);

    switch (SeqNr)
    {
        case 30:
            parseCommand("*MF04");      // Magic Flicker for 4 seconds
        break;    
        default:
        break;
    }

    // Finally GOOOOO
    Sequencer.startSequence();
}