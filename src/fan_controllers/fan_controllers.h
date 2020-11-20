#ifndef __MARS_FAN_CONTROLLERS_H__
#define __MARS_FAN_CONTROLLERS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include <Servo.h>

#define ESC_PIN 9
#define UPPER_SERVO_MAP 180
#define LOWER_SERVO_MAP 0
#define UPPER_D_ESC_MAP 180
#define LOWER_D_ESC_MAP 0

class FanController : public MARSCrashableModule {
    public:
        FanController(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "I") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        bool updatePayloadData(bool forceDataUpdate) override;

    public:
        void setSpeed(int steps);
        void writeSpeed();
        int  getSpeed();
        void testRunLoop();

    public:
        int upperESCmap = UPPER_D_ESC_MAP;
        int lowerESCmap = LOWER_D_ESC_MAP;

    private:
        Servo ESC;
        int currentSpeed = 0;

};

#endif