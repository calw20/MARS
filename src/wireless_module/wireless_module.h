#ifndef __MARS_WIRELESS_MODULE_H__
#define __MARS_WIRELESS_MODULE_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

//#include "../pressure_sensor/pressure_senor.h"
#include "../stepper_motor/stepper_motor.h"
//#include "../sd_card/sd_card.h"
//#include "../accell_gyro/accell_gyro.h"
//#include "../fan_controllers/fan_controllers.h"
//#include "../gps/gps.h"

#include "../wireless_radio/wireless_datatypes.h"
#include "wireless_settings.h"
#include "../wireless_radio/wireless_radio.h"


class WirelessModule : public WirelessRadio, public MARSCrashableModule {
    public:
        WirelessModule(RootModule &uncrashableParent, StepperMotor* ptrStepper, bool addSelfToParent = true);
        bool init() override { return begin();} ;
        void printDebug(String = "R") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        //bool updatePayloadData(bool forceDataUpdate) override;

    public:
        bool handleWirelessCommand(WirelessCommands cmd, void *buffer);
        bool waitForHandledCommand(unsigned long timeout = RADIO_TIMEOUT);
    
    private:
        StepperMotor* ptrSandwitch;
};

#endif