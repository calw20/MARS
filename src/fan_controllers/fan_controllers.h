#ifndef __MARS_FAN_CONTROLLERS_H__
#define __MARS_FAN_CONTROLLERS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include <Servo.h>

#define ESC_PIN 9

class FanController : public CrashableModule {
    public:
        FanController(UnCrashable &uncrashableParent, bool addSelfToParent = true) 
            : CrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "I") override;
        void genericError(const char* func, const char* file, u16 failLine) override;

    public:
        void setSpeed(int steps);
        int  getSpeed();

    private:
        Servo ESC;
        int currentSpeed = 0;      
};

#endif