#ifndef __MARS_WIRELESS_RADIO_H__
#define __MARS_WIRELESS_RADIO_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
//#include "../mars_root_module/mars_root_module.h"

#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include "wireless_datatypes.h"
#include "wireless_settings.h"
//#include "../wireless_module/wireless_module.h"

//[TODO] Pull Nano pinout from docs/board.
//I.G CE/CS; Mega2560 [49,48], ESP8266 [2,4], Nano [??/??]

#ifndef CE_PIN
    #define CE_PIN 8
#endif

#ifndef CS_PIN
    #define CS_PIN 9
#endif

//Default to being the base station
//This is the index of NODE_ADDRESSES
#ifndef NODE_ADDRESS
    #define NODE_ADDRESS 0
#endif

//See "Octal Addressing and Topology" in https://nrf24.github.io/RF24Network/
#if !defined(NODE_ADDRESSES) || !defined(NODE_ADDRESSES_LENGTH)
    #define NODE_ADDRESSES {00, 01}
    #define NODE_ADDRESSES_LENGTH 2
#endif

//Channel Range; [0, 125]
#ifndef RF_RADIO_CHANNEL
    #define RF_RADIO_CHANNEL 90
#endif

#ifndef RADIO_TIMEOUT
    #define RADIO_TIMEOUT 100UL //ms
#endif

#ifndef WAIT_FOR_RESEND_REQUEST
    #define WAIT_FOR_RESEND_REQUEST true
#endif

#ifndef RESEND_REQUEST_TIMEOUT
    #define RESEND_REQUEST_TIMEOUT 100UL //ms
#endif

class WirelessRadio {
    friend class WirelessModule;
    public:
        WirelessRadio();
        bool begin();
        uint8_t updateNetwork();
        
        size_t commandDataSize(WirelessCommands cmd);
        bool sendCommand(WirelessCommands cmd, void* data = nullptr);
        bool sendCommand(uint16_t node_address, WirelessCommands cmd, void* data);
        WirelessCommands waitForCommand(unsigned long timeout = RADIO_TIMEOUT, void* data = nullptr); //May not have timeout?
        
        size_t responseDataSize(WirelessResponses rsp);
        bool sendResponse(WirelessResponses rsp, void* data = nullptr);
        bool sendResponse(uint16_t node_address, WirelessResponses cmd, void* data);
        WirelessResponses waitForResponse(void* data = nullptr, unsigned long timeout = RADIO_TIMEOUT); //May not have timeout?
        
    protected:
        size_t maxDataBufferSize();

    protected:
        uint16_t node_addresses[NODE_ADDRESSES_LENGTH] = NODE_ADDRESSES;
        RF24 *pRadio;
        RF24Network *pNetwork;
        void *dataBuffer;
};



#endif