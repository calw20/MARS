#include "wireless_radio.h"

WirelessRadio::WirelessRadio(){
    pRadio = new RF24(CE_PIN, CS_PIN); 
    dataBuffer = malloc(maxDataBufferSize());
};

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
    //This is the largest thing to handle wirelessly, and insted of doing
    //some mad enum looping stuff, I'll just make the buffer huge.
    return sizeof(payloadData);

    /*size_t maxValue = 0;
    //WirelessCommands  cmdList[] = { WirelessCommands::NoCommand, WirelessCommands::ResendData, WirelessCommands::SendState, WirelessCommands::SendArmState, WirelessCommands::ArmPayload, WirelessCommands::ForceFilterRotation, WirelessCommands::ResetSystem, WirelessCommands::AcceptSystemReset, WirelessCommands::PerformSelfTest };
    WirelessResponses rspList[] = { WirelessResponses::NoResponse, WirelessResponses::SystemInitialized, WirelessResponses::SystemReinitialized, WirelessResponses::SystemArmed, WirelessResponses::SystemState };

    //Currently there is no command that has a return value
    //for (const WirelessCommands &cmd : cmdList)
    //    maxValue = max(maxValue, commandDataSize(cmd));

    for (const WirelessResponses &rsp : rspList)
        maxValue = max(maxValue, responseDataSize(rsp));

    return maxValue;*/
}

//[TODO] Re-order cmd/rsp & timeout??

size_t WirelessRadio::commandDataSize(WirelessCommands cmd){
    size_t dataSize = 0;
    //[NOTE] Unless otherwise specified, the default is to send no data with the command. 
    switch (cmd){
    case WirelessCommands::PerformSelfTest:
        dataSize = sizeof(PerformTest);
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
        if (WAIT_FOR_RESEND_REQUEST && WirelessCommands::ResendData == waitForCommand(RESEND_REQUEST_TIMEOUT))
            returnValue &= pRadio->write(data, dataSize);
    }
    
    return returnValue;
}

WirelessCommands WirelessRadio::waitForCommand(unsigned long timeout, void* data){
    WirelessCommands cmd = WirelessCommands::NoCommand;
    unsigned long timeoutStart = millis();
    while (pRadio->available() || millis()-timeoutStart < timeout || timeout == 0)
        if (pRadio->available()){
            pRadio->read(&cmd, sizeof(WirelessCommands));
            break;
        }

    size_t dataSize = commandDataSize(cmd);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pRadio->available() || millis()-timeoutStart < timeout || timeout == 0)
            if (pRadio->available()){
                pRadio->read(data, dataSize);
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

    case WirelessResponses::SystemReinitialized:
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
        if (WAIT_FOR_RESEND_REQUEST && WirelessCommands::ResendData == waitForCommand(RESEND_REQUEST_TIMEOUT)) //Real short timeout
            returnValue &= pRadio->write(data, dataSize);
    }
    
    return returnValue;
}

WirelessResponses WirelessRadio::waitForResponse(void* data, unsigned long timeout){
    WirelessResponses rsp = WirelessResponses::NoResponse;
    unsigned long timeoutStart = millis();
    while (pRadio->available() || millis()-timeoutStart < timeout || timeout == 0)
        if (pRadio->available()){
            pRadio->read(&rsp, sizeof(WirelessResponses));
            break;
        }
    
    size_t dataSize = responseDataSize(rsp);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pRadio->available() || millis()-timeoutStart < timeout || timeout == 0)
            if (pRadio->available()){
                pRadio->read(data, dataSize);
                break;
            }
    }

    return rsp;

}