#ifndef __MARS_STEPPER_TESTS_H__
#define __MARS_STEPPER_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "../stepper_motor/stepper_motor.h"

#ifndef TEST_STEP_TIMEOUT
    #define TEST_STEP_TIMEOUT 500 //ms
#endif

#ifndef TEST_STEPS_FOR_TINY_ROTATION
    #define TEST_STEPS_FOR_TINY_ROTATION 10
#endif

bool stepperTestRotation(Stepper testStepper, long int steps, unsigned long timeout);
bool stepperTestFullRotation(Stepper testStepper);
bool stepperTestOneRotation(Stepper testStepper);
bool stepperTestTinyRotation(Stepper testStepper);

#endif