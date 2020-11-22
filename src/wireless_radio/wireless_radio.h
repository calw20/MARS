#ifndef __MARS_WIRELESS_RADIO_H__
#define __MARS_WIRELESS_RADIO_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include <SPI.h>
#include <RF24.h>
#include "wireless_datatypes.h"
#include "../wireless_module/wireless_settings.h"
#include "../wireless_module/wireless_module.h"

#ifndef CE_PIN
    #define CE_PIN 8
#endif

#ifndef CS_PIN
    #define CS_PIN 9
#endif

#ifndef RADIOMODE
    #define RADIOMODE 0
#endif

#ifndef RADIO_TIMEOUT
    #define RADIO_TIMEOUT 100UL //ms
#endif


class WirelessRadio{
    friend class WirelessModule;
    public:
        WirelessRadio() : RF24(CE_PIN, CS_PIN){};
        bool begin();
        
        bool sendCommand(WirelessCommands cmd, void* data = nullptr);
        WirelessCommands waitForCommand(void* data = nullptr, unsigned long timeout = RADIO_TIMEOUT); //May not have timeout?
        bool sendResponse(WirelessResponses rsp, void* data = nullptr);
        WirelessResponses waitForResponse(void* data = nullptr, unsigned long timeout = RADIO_TIMEOUT); //May not have timeout?


    protected:
        byte radioPipeNames[2][8] = RADIOPIPENAMES;
        RF24 radio;
}



#endif