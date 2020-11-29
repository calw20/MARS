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
    ArmPayload, //None
    ForceFilterRotation, //None 
    ResetSystem, //None
    AcceptSystemReset, //None
};

enum class WirelessResponses : uint16_t {
    NoResponse, //None
    SystemInitialized, //Unsigned Long
    SystemArmed, //Bool
    SystemState, //PayloadData
};


/*
template typename<T>
struct messageHeader {
    T id;
    //size_t size = 0; //All sizes are known??
}

template typename<T>
class message {
    public:
        messageHeader<T> header;
        virtual size_t  size();
}

typedef messageHeader<WirelessCommands> commandHeader;
typedef messageHeader<WirelessResponses> responseHeader;
/ *
typedef message<WirelessCommands> commandMessage;
typedef message<WirelessResponses> responseMessage;

class MsgPayloadData : responseMessage {
    public:
        MsgPayloadData(payloadData sPData) { pData = sPData; header.id = PayloadData; header.size = size()};
        size_t size(){ return sizeof(pData); };
        payloadData pData;
}

class MsgBool : responseHeader {
    public:
        MsgBool(bool sendValue, responseHeader sentHeader){ value = sendValue; header = sentHeader; };
        size_t size(){ return sizeof(value); };
        bool value;
}*/

#endif