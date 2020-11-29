#include "wireless_radio.h"

//Initialize the radio
bool WirelessRadio::begin(){
    pRadio->begin();
    pRadio->setPALevel(RF24_PA_HIGH); //This *may* want to be RF24_PA_LOW
    //Setup read/write pipes
    pRadio->openWritingPipe(radioPipeNames[0 + RADIOMODE]);
    pRadio->openReadingPipe(1, radioPipeNames[1 - RADIOMODE]);
    pRadio->startListening();

    //Send Init Code
    pRadio->stopListening();
    unsigned long cTime = millis();
    sendResponse(WirelessResponses::SystemInitialized, &cTime);

    pRadio->startListening();

    DBG_FPRINT("Initialized Radio");
}

size_t WirelessRadio::maxDataBufferSize(){
    return sizeof(payloadData);
}

//[TODO] Re-order cmd/rsp & timeout??

size_t WirelessRadio::commandDataSize(WirelessCommands cmd){
    size_t dataSize = 0;
    
    switch (cmd){
    case WirelessCommands::NoCommand:
    case WirelessCommands::ResendData:
    case WirelessCommands::SendState:
    case WirelessCommands::ArmPayload:
    case WirelessCommands::ForceFilterRotation:
    case WirelessCommands::ResetSystem:
    case WirelessCommands::AcceptSystemReset:
        break;
    default:
        break;
    }

    return dataSize;
}

bool WirelessRadio::sendCommand(WirelessCommands cmd, void* data){
    size_t dataSize = commandDataSize(cmd);

    bool returnValue = true;
    returnValue &= pRadio->write(&cmd, sizeof(cmd));
    if (dataSize > 0 && data){
        returnValue &= pRadio->write(data, dataSize);
        if (WirelessCommands::ResendData == waitForCommand(nullptr, RESEND_REQUEST_TIMEOUT))
            returnValue &= pRadio->write(data, dataSize);
    }
    
    return returnValue;
}

WirelessCommands WirelessRadio::waitForCommand(void* data, unsigned long timeout){
    WirelessCommands cmd = WirelessCommands::NoCommand;
    unsigned long timeoutStart = millis();
    while (pRadio->available() || millis()-timeoutStart < timeout)
        if (pRadio->available()){
            pRadio->read(&cmd, sizeof(WirelessCommands));
            break;
        }

    size_t dataSize = commandDataSize(cmd);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pRadio->available() || millis()-timeoutStart < timeout)
            if (pRadio->available()){
                pRadio->read(&cmd, sizeof(WirelessCommands));
                break;
            }
    }

    return cmd;

}

size_t WirelessRadio::responseDataSize(WirelessResponses rsp){
    size_t dataSize = 0;
    switch (rsp){
    case WirelessResponses::NoResponse:
        break;
    case WirelessResponses::SystemInitialized:
        dataSize = sizeof(unsigned long);
        break;
    case WirelessResponses::SystemArmed:
        dataSize = sizeof(bool);
        break;
    case WirelessResponses::SystemState:
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
    returnValue &= pRadio->write(&cmd, sizeof(cmd));
    if (dataSize > 0 && data){
        returnValue &= pRadio->write(data, dataSize);
        if (WirelessCommands::ResendData == waitForCommand(nullptr, RESEND_REQUEST_TIMEOUT)) //Real short timeout
            returnValue &= pRadio->write(data, dataSize);
    }
    
    return returnValue;
}

WirelessResponses WirelessRadio::waitForResponse(void* data, unsigned long timeout){
    WirelessResponses rsp = WirelessResponses::NoResponse;
    unsigned long timeoutStart = millis();
    while (pRadio->available() || millis()-timeoutStart < timeout)
        if (pRadio->available()){
            pRadio->read(&rsp, sizeof(WirelessResponses));
            break;
        }
    
    size_t dataSize = responseDataSize(rsp);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pRadio->available() || millis()-timeoutStart < timeout)
            if (pRadio->available()){
                pRadio->read(data, dataSize);
                break;
            }
    }

    return rsp;

}