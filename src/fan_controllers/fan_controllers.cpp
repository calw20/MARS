#include "fan_controllers.h"

//Makes things look neater I guess?
bool FanController::init(){
    /*
    pinMode(STEP_PULSE_PIN, OUTPUT); //StepMC Pulse Pin
    pinMode(STEP_DIREC_PIN, OUTPUT); //StepMC Direction Pin
    pinMode(STEP_ENABL_PIN, OUTPUT); //StepMC Enable Pin
    */
    DBG_FPRINT("Initializing Fan Controllers...");
    ESC.attach(ESC_PIN,1000,2000);
    DBG_FPRINT("Arming Fan Controllers...");
    ESC.setSpeed(0);
    DBG_FPRINT("Please Wait...");
    delay(1500); //Wait for amring
    DBG_FPRINT("Armed and Operational.");
    DBG_FPRINT("Initialized Fan Controllers.");

    return true;
}

void FanController::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("################## Fan Controllers Error Info ##################");
    DBG_FPRINTF("The Fan Controllers have experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void FanController::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    if (CHK_LETTER("-H")) printValues = "I";

    if (CHK_LETTER("H")) DBG_FPRINTLN("===================== Fan Controllers Info =====================");
    
    if (CHK_LETTER("I")){
        DBG_FPRINT_SVLN("Current Fan Speed: ", currentSpeed);
    }
}

int FanController::getSpeed(){
    //[TODO] Make Consts - The inverse of below
    return map(currentSpeed, 0, 180, 0, 180);
}

void FanController::setSpeed(int newSpeed){
    //[TODO] Make Consts
    currentSpeed = map(newSpeed, 0, 180, 0, 180);
    ESC.write(currentSpeed);
}