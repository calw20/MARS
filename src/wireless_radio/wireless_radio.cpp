#include "wireless_radio.h"

WirelessRadio::WirelessRadio(){
    pRadio   = new RF24(CE_PIN, CS_PIN);
    pNetwork = new RF24Network(*pRadio); 
    dataBuffer = malloc(maxDataBufferSize());
};

//Initialize the radio
bool WirelessRadio::begin(){
    //Start the radio   
    if (!pRadio->begin())
        return false;
    pRadio->setPALevel(RF24_PA_HIGH); //This *may* want to be RF24_PA_LOW

    //Setup the network
    pNetwork->begin(RF_RADIO_CHANNEL, node_addresses[NODE_ADDRESS]);

    //Send Init Code
    unsigned long cTime = millis();
    sendResponse(WirelessResponses::SystemInitialized, &cTime);

    DBG_FPRINT("Initialized Radio");
    return pRadio->isChipConnected();
}

uint8_t WirelessRadio::updateNetwork(){
    return pNetwork->update();    
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
    //For the sake of "simplicity" by default only send to the next node in the list.
    uint16_t node_address = node_addresses[(NODE_ADDRESS + 1) % NODE_ADDRESSES_LENGTH];
    return sendCommand(node_address, cmd, data);
}

bool WirelessRadio::sendCommand(uint16_t node_address, WirelessCommands cmd, void* data){
    //updateNetwork();
    RF24NetworkHeader header(node_address);
    size_t dataSize = commandDataSize(cmd);
    
    bool returnValue = true;
    returnValue &= pNetwork->write(header, &cmd, sizeof(cmd));
    if (dataSize > 0 && data){
        returnValue &= pNetwork->write(header, data, dataSize);
        if (WAIT_FOR_RESEND_REQUEST && WirelessCommands::ResendData == waitForCommand(RESEND_REQUEST_TIMEOUT))
            returnValue &= pNetwork->write(header, data, dataSize);
    }

    return returnValue;
}

WirelessCommands WirelessRadio::waitForCommand(unsigned long timeout, void* data){
    //updateNetwork();
    RF24NetworkHeader header;

    WirelessCommands cmd = WirelessCommands::NoCommand;
    unsigned long timeoutStart = millis();
    while (pNetwork->available() || millis()-timeoutStart < timeout || timeout == 0){
        updateNetwork();
        if (pNetwork->available()){
            pNetwork->read(header, &cmd, sizeof(WirelessCommands));
            break;
        }
    }

    size_t dataSize = commandDataSize(cmd);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pNetwork->available() || millis()-timeoutStart < timeout || timeout == 0){
            updateNetwork();
            if (pNetwork->available()){
                pNetwork->read(header, data, dataSize);
                break;
            }
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
    //For the sake of "simplicity" by default only send to the next node in the list.
    uint16_t node_address = node_addresses[(NODE_ADDRESS + 1) % NODE_ADDRESSES_LENGTH];
    return sendResponse(node_address, cmd, data);
}

bool WirelessRadio::sendResponse(uint16_t node_address, WirelessResponses cmd, void* data){
    //updateNetwork();
    RF24NetworkHeader header(node_address);
    size_t dataSize = responseDataSize(cmd);

    bool returnValue = true;
    returnValue &= pNetwork->write(header, &cmd, sizeof(cmd));
    if (dataSize > 0 && data){
        delay(10);
        returnValue &= pNetwork->write(header, data, dataSize);
        if (WAIT_FOR_RESEND_REQUEST && WirelessCommands::ResendData == waitForCommand(RESEND_REQUEST_TIMEOUT)) //Real short timeout
            returnValue &= pNetwork->write(header, data, dataSize);
    }

    return returnValue;
}

WirelessResponses WirelessRadio::waitForResponse(void* data, unsigned long timeout){
    //updateNetwork();
    RF24NetworkHeader header;
    
    WirelessResponses rsp = WirelessResponses::NoResponse;
    unsigned long timeoutStart = millis();
    while (pNetwork->available() || millis()-timeoutStart < timeout || timeout == 0){
        updateNetwork();
        if (pNetwork->available()){
            pNetwork->read(header, &rsp, sizeof(WirelessResponses));
            break;
        }
    }

    size_t dataSize = responseDataSize(rsp);
    if (data && dataSize > 0){
        timeoutStart = millis();
        while (pNetwork->available() || millis()-timeoutStart < timeout || timeout == 0){
            updateNetwork();
            if (pNetwork->available()){
                pNetwork->read(header, data, dataSize);
                break;
            }
        }
    }

    return rsp;
}