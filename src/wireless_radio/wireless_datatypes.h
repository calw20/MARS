#ifndef __MARS_WIRELESS_DATA_H__
#define __MARS_WIRELESS_DATA_H__

#include "Arduino.h"

#include "../mars_root_module/data_types.h"

//[TODO] Add 'const char* stateStr[] =' arrays or F() switch cases in a function 

enum class WirelessCommands : uint16_t {
    NoCommand,  //None
    ResendData, //None
    SendState, //None
    ArmPayload, //None
    SendArmState, //None
    ForceFilterRotation, //None 
    PerformSelfTest, //None
    ResetSystem, //None
    AcceptSystemReset, //None
};

enum class WirelessResponses : uint16_t {
    NoResponse, //None
    SystemInitialized, //Unsigned Long
    SystemReinitialized, //Unsigned Long //![WARN] To be Depreciated
    SystemReinitializing, //Unsigned Long
    SystemArmed, //Bool
    SystemState, //PayloadData,
    CommandSucceeded, //Bool
};

//May want to move this to the test module
enum class PerformTest : uint16_t {
    NoTest,
    FullCheck,
    FullStepperCheck,

};

#endif