#include "stepper_tests.h"

bool stepperTestRotation(StepperMotor testStepper, long int steps, unsigned long timeout){
    DBG_PRINTFN("[Stepper Test] Rotating %ld steps...", steps);
    testStepper.rotateAndSignal(steps);
    DBG_FPRINTLN("[Stepper Test] Rotated, wating for timout...");
    delay(timeout);
    DBG_FPRINTLN("[Stepper Test] Waited, reverseing rotation...");
    testStepper.rotateAndSignal(-steps);
    DBG_FPRINTLN("[Stepper Test] Rotated. System should be reset to original position.");
    
    return true;
}

bool stepperTestFullRotation(StepperMotor testStepper){
    DBG_FPRINTLN("[Stepper Test] Full Rotation Test");
    DBG_FPRINTLN("[WARNING] This WILL expose ALL samples to air.");
    return stepperTestRotation(testStepper, ALT_SAMPLE_COUNT*STEPS_PER_ROTATION, TEST_STEP_TIMEOUT);
}

bool stepperTestOneRotation(StepperMotor testStepper){
    DBG_FPRINTLN("[Stepper Test] One Rotation Test");
    DBG_FPRINTLN("[WARNING] This WILL expose at least ONE sample to air.");
    return stepperTestRotation(testStepper, STEPS_PER_ROTATION, TEST_STEP_TIMEOUT);
}

bool stepperTestTinyRotation(StepperMotor testStepper){
    DBG_FPRINTLN("[Stepper Test] Tiny Rotation Test");
    DBG_FPRINTLN("[WARNING] This MAY expose some samples to air.");
    return stepperTestRotation(testStepper, TEST_STEPS_FOR_TINY_ROTATION, TEST_STEP_TIMEOUT);
}
