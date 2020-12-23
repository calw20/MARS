#include "stepper_tests.h"

bool stepperTestRotation(Stepper testStepper, long int steps, unsigned long timeout){
    DBG_FPRINTLN("[Stepper Test] Full Rotation Test");
    DBG_FPRINTLN("[WARNING] This MAY expose samples to air.");
    DBG_PRINTFN("[Stepper Test] Rotating %ld steps...", steps);
    testStepper.rotateAndSignal(steps);
    DBG_FPRINTLN("[Stepper Test] Rotated, wating for timout...");
    delay(timeout);
    DBG_FPRINTLN("[Stepper Test] Waited, reverseing rotation...");
    testStepper.rotateOneFilter(-steps);
    DBG_FPRINTLN("[Stepper Test] Rotated. System should be reset to original position.");
    
    return true;
}

bool stepperTestFullRotation(Stepper testStepper){
    DBG_FPRINTLN("[WARNING] This WILL expose ALL samples to air.");
    return stepperTestRotation(testStepper, ALT_SAMPLE_COUNT*STEPS_PER_ROTATION, TEST_STEP_TIMEOUT);
}

bool stepperTestOneRotation(Stepper testStepper){
    DBG_FPRINTLN("[WARNING] This WILL expose at least ONE sample to air.");
    return stepperTestRotation(testStepper, STEPS_PER_ROTATION, TEST_STEP_TIMEOUT);
}

bool stepperTestTinyRotation(Stepper testStepper){
    DBG_FPRINTLN("[WARNING] This MAY expose some samples to air.");
    return stepperTestRotation(testStepper, TEST_STEPS_FOR_TINY_ROTATION, TEST_STEP_TIMEOUT);
}
