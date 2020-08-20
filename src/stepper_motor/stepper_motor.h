#ifndef __MARS_STEPPER_MOTOR_H__
#define __MARS_STEPPER_MOTOR_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
//#include "../mars_root_module/mars_root_module.h"

#define LED_PIN LED_BUILTIN     //Diagnostic LED Pin
#define STEP_PULSE_PIN 7        //Stepper Motor Controller Pulse Pin (pwm)
#define STEP_DIREC_PIN 6        //Stepper Motor Controller Direction Pin
#define STEP_ENABL_PIN 5        //Stepper Motor Controller Enable Pin

#define MICRO_STEP_DELAY 50

const int filterHeights[] = FILTER_HEIGHTS;

class StepperMotor : public CrashableModule {
    public:
        StepperMotor(UnCrashable &uncrashableParent, bool addSelfToParent = true) 
            : CrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "Z") override;
        void genericError(const char* func, const char* file, u16 failLine) override;

    public:
        void rotateSandwitch(long int steps);
        void nextFilter(u8& currentFilter);
        u8 getCurrentFilter();

    private:
        u8 currentFilter = 0;
        u8 maxFilterNumber = 0;
        
};

#endif