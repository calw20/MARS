#ifndef __MARS_WIRELESS_MODULE_H__
#define __MARS_WIRELESS_MODULE_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#define RADIOMODE 0

class WirelessModule : public MARSCrashableModule {
    public:
        WirelessModule(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "R") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        //bool updatePayloadData(bool forceDataUpdate) override;

    public:
        /*WirelessCommands getCommand();
        WirelessCommands waitForCommand();
        bool sendData();
        bool sendCommand();*/
        bool waitForData();


    private:
        byte radioPipeNames[2][8] = RADIOPIPENAMES;
        RF24* radio;

};

#endif