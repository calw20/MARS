#include "system_tests.h"

SystemTestHandler::SystemTestHandler(RootModule* ptrMARSRoot,
    PressureSensor* ptrPrsSensor, SDCardAdapter* ptrSdCard,
    StepperMotor* ptrStepper, AccellGyro* ptrAccellGyro, 
    FanController* ptrFanController, GPSModule* ptrGPSRadio, 
    WirelessModule* ptrWirelessModule, 
    char newlineChar, char delimiterChar)
    : SerialTerminal(newlineChar, delimiterChar) {
    
    //Setup Pointers
    prsSensor = ptrPrsSensor;
    sdCard = ptrSdCard;
    sandwitch = ptrStepper;
    accellGyro = ptrAccellGyro;
    fanController = ptrFanController;
    gpsRadio = ptrGPSRadio;
    wirelessModule = ptrWirelessModule;

    //Setup Serial Commands
    setDefaultHandler(unknownCommand);
    addCommand("?", cmdHelp);
    addCommand("help", cmdHelp);
    addCommand("info", cmdInfo);
    addCommand("reset", cmdReset);
    addCommand("stepper", cmdStepper);
    addCommand("fans", cmdFans);
    addCommand("leds", cmdLEDs);
    addCommand("wirelessTest", cmdWirelessTest);

};

void SystemTestHandler::testMode(){
    DBG_FPRINTLN("Entering Test Mode.");
    DBG_FPRINTLN("Enter 'help' for useage.");
    inTestMode = true;
    while(inTestMode){
        readSerial();
    }
}

void SystemTestHandler::unknownCommand(const char *command){
    DBG_FPRINTLN("Unknown command: ");
    DBG_PRINTLN(command);
}

//Help Menu
void SystemTestHandler::cmdHelp(){
    DBG_FPRINTLN("Serial terminal usage:");
    DBG_FPRINTLN("  help or ?       Print this usage");
    DBG_FPRINTLN("  info <sensor>   Print sensor info, use \'info help\' for more infomation.");
    DBG_FPRINTLN("  reset           Reset the system")
    DBG_FPRINTLN("  stepper <steps> Test Stepper");
    DBG_FPRINTLN("  fans <speed>    Test Fan Controllers / EDFs");
    DBG_FPRINTLN("  leds            Test LEDs");
    DBG_FPRINTLN("  wirelessTest    Test the Wireles Comms");

    //DBG_FPRINTLN("  ");
    /*DBG_FPRINTLN("  off                   Turn LED off");
    DBG_FPRINTLN("  hello [name] [age]    Print your name and age");
    DBG_FPRINTLN("  s <arg 1>..<arg N>    Print string arguments");
    DBG_FPRINTLN("  i <arg>               Print decimal or hex argument");*/
}

void SystemTestHandler::cmdReset(){
    Serial.print(F("About to reset system"));
    for(int i; i < 4; i++)
        Serial.print(F("."));
    
    resetSystem();
}

void SystemTestHandler::cmdStepper(){
    char *arg;
    int val;

    // Get argument
    arg = term.getNext();
    if (arg == NULL) {
        DBG_FPRINTLN("Steps not specified.");
        return;
    }
    val = atoi(arg);

    DBG_FPRINTLN("Testing Stepper...");
    stepperTestRotation(sandwitch*, val, 1000);
}

void SystemTestHandler::cmdFans(){
    char *arg;
    int val;

    // Get argument
    arg = term.getNext();
    if (arg == NULL) {
        DBG_FPRINTLN("Speed not specified.");
        return;
    }
    val = atoi(arg);

    DBG_FPRINTLN("Testing Fans...");
    fanTest(fanController*, val, 5000);
}

//[TODO] Create LED Tests
void SystemTestHandler::cmdLEDs(){

}

//[TODO] Create Wireless Tests
void SystemTestHandler::cmdWirelessTest(){

}

void SystemTestHandler::cmdInfo(){
    char *arg;
    arg = getNext();
    if (strcmp(arg, "info")){
        DBG_FPRINTLN("Available \'info\' arguments are:");
        DBG_FPRINTLN("   all          Print all sensor info.");
        DBG_FPRINTLN("   root         Print root module info.");
        DBG_FPRINTLN("   pressure     Print pressure sensor info.");
        DBG_FPRINTLN("   sdcard       Print sdcard info.");
        DBG_FPRINTLN("   stepper      Print sandwitch info.");
        DBG_FPRINTLN("   accellgyro   Print accellgyro info.");
        DBG_FPRINTLN("   fans         Print ESC / EDF info.");
        DBG_FPRINTLN("   gps          Print GPS Radio info.");
        DBG_FPRINTLN("   wireless     Print Wireless Comms info.");
    }
    else if (strcmp(arg, "all"))
        marsRoot->printDebug();
    else if (strcmp(arg, "root")){
        marsRoot->inError = true;
        marsRoot->printDebug();
        marsRoot->inError = false;
    }
    else if (strcmp(arg, "pressure"))
        prsSensor->printDebug();
    else if (strcmp(arg, "sdcard"))
        sdCard->printDebug();
    else if (strcmp(arg, "stepper"))
        sandwitch->printDebug();
    else if (strcmp(arg, "accellgyro"))
        accellGyro->printDebug();
    else if (strcmp(arg, "fans"))
        fanController->printDebug();
    else if (strcmp(arg, "gps"))
        gpsRadio->printDebug();
    else if (strcmp(arg, "wireless"))
        wirelessModule->printDebug();
    else
        DBG_FPRINT_SVLN("Unknown Argument: ", arg);
        DBG_FPRINTLN("Use \'info help\' for more infomation.");
}