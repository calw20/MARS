#include "wireless_radio.h"

//Initialize the radio
bool WirelessRadio::begin(){
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH); //This *may* want to be RF24_PA_LOW
    //Setup read/write pipes
    radio.openWritingPipe(radioPipeNames[0 + RADIOMODE]);
    radio.openReadingPipe(1, radioPipeNames[1 - RADIOMODE]);
    radio.startListening();

    //Send Init Code
    radio.stopListening();
    WirelessResponses sendCode = SystemInitialized;
    unsigned long cTime = millis();
    
    radio.write(&cTime, sizeof(cTime));
    radio.startListening();

    DBG_FPRINT("Initialized Radio");
}

//[TODO] Re-order cmd/rsp & timeout??

size_t WirelessRadio::commandDataSize(WirelessCommands cmd){
    size_t dataSize = 0;
    
    switch (cmd){
    case SystemInitialized:
    case NoCommand:
    case SendState:
    case ArmPayload:
    case ForceFilterRotation:
    case ResetSystem:
    case AcceptSystemReset:
        break;
    default:
        break;
    }

    return dataSize;
}

bool WirelessRadio::sendCommand(WirelessCommands cmd, void* data){
    size_t dataSize = commandDataSize(cmd);

    bool returnValue = true;
    returnValue &= radio.write(&cmd, sizeof(cmd));
    if (dataSize > 0 && data)
        returnValue &= radio.write(data, dataSize);
    
    return returnValue;
}

WirelessCommands waitForCommand(void* data, unsigned long timeout){
    WirelessCommands cmd = NoCommand;
    unsigned long timeoutStart = millis();
    while (radio.available() || millis()-timeoutStart < timeout)
        if (radio.available()){
            radio.read(&cmd, sizeof(WirelessCommands));
            break;
        }

    size_t dataSize = commandDataSize(cmd);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (radio.available() || millis()-timeoutStart < timeout)
            if (radio.available()){
                radio.read(&cmd, sizeof(WirelessCommands));
                break;
            }
    }

    return cmd;

}

size_t WirelessRadio::responseDataSize(WirelessResponses rsp){
    size_t dataSize = 0;
    switch (cmd){
    case NoResponse:
        break;
    case SystemInitialized:
        dataSize = sizeof(unsigned long);
        break;
    case SystemArmed:
        dataSize = sizeof(bool);
        break;
    case SystemState:
        dataSize = sizeof(payloadData);
        break;
    default:
        break;
    }
    return dataSize;
}

bool WirelessRadio::sendResponse(WirelessResponses cmd, void* data){
    size_t dataSize = responseDataSize(cmd);

    bool returnValue = true;
    returnValue &= radio.write(&cmd, sizeof(cmd));
    if (dataSize > 0 && data)
        returnValue &= radio.write(data, dataSize);
    
    return returnValue;
}

WirelessResponses waitForResponse(void* data, unsigned long timeout){
    WirelessResponses rsp = NoResponse;
    unsigned long timeoutStart = millis();
    while (radio.available() || millis()-timeoutStart < timeout)
        if (radio.available()){
            radio.read(&rsp, sizeof(WirelessResponses));
            break;
        }
    
    size_t dataSize = responseDataSize(rsp);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (radio.available() || millis()-timeoutStart < timeout)
            if (radio.available()){
                radio.read(data, dataSize);
                break;
            }
    }

    return rsp;

}