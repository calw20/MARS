#include "stepper_motor.h"

//Makes things look neater I guess?
bool StepperMotor::init(){
    pinMode(STEP_PULSE_PIN, OUTPUT); //StepMC Pulse Pin
    pinMode(STEP_DIREC_PIN, OUTPUT); //StepMC Direction Pin
    pinMode(STEP_ENABL_PIN, OUTPUT); //StepMC Enable Pin

    //First thing we need to do is lock the steper motor, last thing we want
    //is the sandwich rotateing.... (•‿•)
    digitalWrite(STEP_ENABL_PIN, HIGH);
    maxFilterNumber = sizeof(filterHeights)/sizeof(*filterHeights);
    marsRoot->data.maxFilterNumber = maxFilterNumber;
    return true;
}

void StepperMotor::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("##################### Sandwitch Error Info #####################");
    DBG_FPRINTF("The Pressure Sensor has experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void StepperMotor::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    if (CHK_LETTER("-H")) printValues = "Z";

    if (CHK_LETTER("H")) DBG_FPRINTLN("======================== Sandwitch Info ========================");
    
    if (CHK_LETTER("Z")){
        DBG_FPRINT_SVLN("Sandwitch currently at sample: ", currentFilter);
        DBG_FPRINT_SVLN("Maximum number of samples: ", maxFilterNumber);
        DBG_FPRINT("Micro Step Delay currently: "); DBG_PRINT(MICRO_STEP_DELAY); DBG_FPRINTLN(" ms");
    }
}

u8 StepperMotor::getCurrentFilter(){
    return currentFilter;
}

//Rotate the sandwich a number of steps
//[TODO] 19-11: This need in person testing as I have disabled fliping the step direction
//\ here and also the negative in STEPS_PER_ROTATION /payload_settings.h ¯\_(ツ)_/¯
void StepperMotor::rotateSandwitch(long int steps){
    //Need to flip steps so that it goes the right direction
    //steps *= -1;

    long int absSteps = abs(steps); //Argh this is a macro and for "speed" reasons Ill do this 
        //it could be more memory efficient to use the macro everywhere but meh, its a mega
    
    //[TODO] Check this works...
    DBG_FPRINTFN("Sandwitch needs to move ", "%li steps %s.", 
        absSteps, (steps/absSteps + 1) ? "Forward" : "Backwards");
    
    digitalWrite(STEP_DIREC_PIN, (steps/absSteps + 1)); //If steps is negative then rotate backwards.
    //Send the steps
    for (int i=0; i <= absSteps; i++){
        digitalWrite(STEP_PULSE_PIN, HIGH);  //Set pulse high
        delayMicroseconds(MICRO_STEP_DELAY); //Wait
        //delay(30);
        digitalWrite(STEP_PULSE_PIN, LOW);   //Set pulse low
        delayMicroseconds(MICRO_STEP_DELAY); //Wait
        //delay(30);
    }
}

bool StepperMotor::nextFilter(){
    if (isAbleToRotate()) {
        DBG_FPRINT_SVLN("Currently on filter: ", currentFilter);
        currentFilter++;
        rotateAndSignal(STEPS_PER_ROTATION);
        DBG_FPRINT_SVLN("Now on filter: ", currentFilter);
        return true;
    } else {
        DBG_FPRINTLN("Request to rotate, however already completed one full turn!");
        return false;
    }
}

void StepperMotor::rotateAndSignal(long int steps){
        //Change the LED colour when rotation is requested
        //[Thought] nextFilter is more of a MARS func then rotate
        LEDColours currentColour = marsRoot->cLED1->getColour();
        marsRoot->cLED1->setColour(LEDColours::YELLOW);
        rotateSandwitch(steps);
        marsRoot->cLED1->setColour(currentColour);
}

bool StepperMotor::isAbleToRotate(){
    return currentFilter < maxFilterNumber;
}

bool StepperMotor::shouldRotate(){
    if (currentFilter < maxFilterNumber){
        return  marsRoot->data.altitude[0] < filterHeights[currentFilter];
    } else {
        return false;
    }
}

bool StepperMotor::updatePayloadData(bool forceDataUpdate){
    marsRoot->data.currentFilter = getCurrentFilter();
    return true;
}