#ifndef __MARS_SYSTEM_TESTS_H__
#define __MARS_SYSTEM_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "../pressure_sensor/pressure_senor.h"
#include "../stepper_motor/stepper_motor.h"
#include "../sd_card/sd_card.h"
#include "../accell_gyro/accell_gyro.h"
#include "../fan_controllers/fan_controllers.h"
#include "../gps/gps.h"
#include "../wireless_module/wireless_module.h"

//Grabbed from [https://github.com/Erriez/ErriezSerialTerminal] on 2020-06-2021
//Licensed under MIT
#include "../ErriezSerialTerminal/ErriezSerialTerminal.h"

#include "stepper_tests.h"
#include "gps_tests.h"
#include "fan_tests.h"
#include "led_tests.h"

static void* gPtrSystemTestHandler;

class WirelessModule;
class SystemTestHandler : public SerialTerminal {
    public:
        SystemTestHandler(char newlineChar = '\n', char delimiterChar = ' ')
            : SerialTerminal(newlineChar, delimiterChar){};
        init(RootModule* ptrMARSRoot, PressureSensor* ptrPrsSensor, 
            SDCardAdapter* ptrSdCard, StepperMotor* ptrStepper, 
            AccellGyro* ptrAccellGyro, FanController* ptrFanController, 
            GPSModule* ptrGPSRadio, 
            WirelessModule* ptrWirelessModule);

    public:
        void testMode();
        //void runTest(PerformTest test);

    private:
        void unknownCommand(const char *command);
        void cmdHelp();
        void cmdInfo();
        void cmdReset();
        void cmdStepper();
        void cmdFans();
        void cmdLEDs();
        void cmdWirelessTest();

    public:
        static void staticUnknownCommand(const char *command);
        static void staticCmdHelp();
        static void staticCmdInfo();
        static void staticCmdReset();
        static void staticCmdStepper();
        static void staticCmdFans();
        static void staticCmdLEDs();
        static void staticCmdWirelessTest();


    private:
        bool inTestMode = false;
        RootModule     *marsRoot;
        PressureSensor *prsSensor;
        SDCardAdapter  *sdCard;
        StepperMotor   *sandwitch;
        AccellGyro     *accellGyro;
        FanController  *fanController;
        GPSModule      *gpsRadio;
        WirelessModule *wirelessModule;

};

#endif