#ifndef __MARS_WIRELESS_DATA_H__
#define __MARS_WIRELESS_DATA_H__

#include "Arduino.h"

#include "../mars_root_module/data_types.h"

//The _payload_ should only ever listen on RX and transmit on TX
//The _control unit_ ("laptop") should do the reverse
#define RADIOPIPENAMES {"MARS_TX","MARS_RX"}

enum class WirelessCommands : uint16_t {
    NoCommand,  //None
    ResendData, //None
    SendState, //None
    SendArmState, //None
    ArmPayload, //None
    ForceFilterRotation, //None 
    ResetSystem, //None
    AcceptSystemReset, //None
    PerformSelfTest, //None
};

enum class WirelessResponses : uint16_t {
    NoResponse, //None
    SystemInitialized, //Unsigned Long
    SystemReinitialized, //Unsigned Long
    SystemArmed, //Bool
    SystemState, //PayloadData,
    CommandSucceeded, //Bool
};

//May want to move this to the test module
enum class PerformTest : uint16_t {
    NoTest,
    FullCheck,
    FullStepperCheck,

}

#endif