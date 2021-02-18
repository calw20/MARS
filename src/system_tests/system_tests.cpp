#include "system_tests.h"

void rootDebugMode(RootModule *marsRoot, SDCardAdapter *sdCard){
    setupSerialCodes();
    DBG_FPRINT("\e[1;1HMARS Payload Data - Debug Mode");
    DBG_FPRINT_SV("\e[2;1HBuild Date: ", F(BUILD_DATE));
    DBG_FPRINT_SV("\e[3;1HBuild Version: ", F(BUILD_VERSION));
    
    bool inDebugMode = true;
    while(inDebugMode){
        if(Serial.available() > 0){
            char inChar = (char)Serial.read();
            Serial.print("\b");
            if (inChar == 27 || inChar == 172 || inChar == 'e')
                inDebugMode = false;
        }
        //marsRoot->updatePayloadData(true);
        //if (sdCard) sdCard->writeCSVData();
        
        //Every 10 seconds or so wipe the screen and re-draw
        if (millis() % 10000 < 100)
            for(int i = 4; i < 31; i++)
                DBG_PRINTF("\e[%i;1H\e[2K", i);

        dumpPayloadDataGrid(marsRoot->data, false);
        
        DBG_FPRINT("\e[30;1HUse the \'ESC\' or \'e\' key to exit.");
    }
    DBG_FPRINT("\e[?25h"); // show cursor
    DBG_FPRINT("\e[?12h"); // enable cursor highlighting
    clearSerial();
    DBG_FPRINTLN("Quit Debug Mode.");
}

void printConsoleChar(){
    DBG_FPRINT("> ");
}

void badArgument(){
    DBG_FPRINTLN("Error! Bad Command, see \'help\' for more info.");
}

//System Test Handler
SystemTestHandler::init(RootModule* ptrMARSRoot,
    PressureSensor* ptrPrsSensor, SDCardAdapter* ptrSdCard,
    StepperMotor* ptrStepper, AccellGyro* ptrAccellGyro, 
    FanController* ptrFanController, GPSModule* ptrGPSRadio, 
    WirelessModule* ptrWirelessModule) {
    
    //Setup Pointers
    gPtrSystemTestHandler = (void*) this;
    prsSensor = ptrPrsSensor;
    sdCard = ptrSdCard;
    sandwitch = ptrStepper;
    accellGyro = ptrAccellGyro;
    fanController = ptrFanController;
    gpsRadio = ptrGPSRadio;
    wirelessModule = ptrWirelessModule;

    //Setup Serial Commands
    setDefaultHandler(SystemTestHandler::staticUnknownCommand);
    setPostCommandHandler(printConsoleChar);
    setSerialEcho(true);

    addCommand("?", SystemTestHandler::staticCmdHelp);
    addCommand("help", SystemTestHandler::staticCmdHelp);
    addCommand("info", SystemTestHandler::staticCmdInfo);
    //addCommand("set", SystemTestHandler::staticCmdSet);
    addCommand("reset", SystemTestHandler::staticCmdReset);
    addCommand("reboot", SystemTestHandler::staticCmdReset); //Shhh this is undocumented
    addCommand("debug", SystemTestHandler::staticCmdDebugMode);
    addCommand("clear", clearSerial);
    addCommand("stepper", SystemTestHandler::staticCmdStepper);
    addCommand("fans", SystemTestHandler::staticCmdFans);
    addCommand("leds", SystemTestHandler::staticCmdLEDs);
    addCommand("wirelessTest", SystemTestHandler::staticCmdWirelessTest);

};

void SystemTestHandler::testMode(){
    DBG_FPRINTLN("Entering Test Mode.");
    DBG_FPRINTLN("Enter 'help' for useage.");
    printConsoleChar();
    inTestMode = true;
    while(inTestMode){
        readSerial();
    }
}

void SystemTestHandler::unknownCommand(const char *command){
    DBG_FPRINT_SVLN("Unknown command: ", command);
}

//Help Menu
void SystemTestHandler::cmdHelp(){
    DBG_FPRINTLN("Serial terminal usage:");
    DBG_FPRINTLN("    help or ?        Print this usage");
    DBG_FPRINTLN("    info <sensor>    Print sensor info, use \'info help\' for more infomation.");
    //DBG_FPRINTLN("    set <sensor> ... Set system values, use \'set help\' for more info.");
    DBG_FPRINTLN("    reset            Reset the system");
    DBG_FPRINTLN("    debug            Enter Debug Mode");
    DBG_FPRINTLN("    clear            Clear the terminal");
    DBG_FPRINTLN("    stepper <steps>  Test Stepper");
    DBG_FPRINTLN("    fans <speed>     Test Fan Controllers / EDFs");
    DBG_FPRINTLN("    leds             Test LEDs");
    DBG_FPRINTLN("    wirelessTest     Test the Wireles Comms");

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
    Serial.println();

    resetSystem();
}

void SystemTestHandler::cmdStepper(){
    char *arg;
    int val;

    // Get argument
    arg = getNext();
    if (arg == NULL) {
        DBG_FPRINTLN("Steps not specified.");
        return;
    }

    val = atoi(arg);

    DBG_FPRINTLN("Testing Stepper...");
    stepperTestRotation(*sandwitch, val, 1000);
}

void SystemTestHandler::cmdFans(){
    char *arg;
    int val;

    // Get argument
    arg = getNext();
    if (arg == NULL) {
        DBG_FPRINTLN("Speed not specified.");
        return;
    }
    val = atoi(arg);

    DBG_FPRINTLN("Testing Fans...");
    fanTest(*fanController, val, 5000);
}

void SystemTestHandler::cmdLEDs(){
    ledTest(*marsRoot);
}

//[TODO] Create Wireless Tests
void SystemTestHandler::cmdWirelessTest(){
    DBG_FPRINTLN("Not Implimented :/ (Yet)");
}

void SystemTestHandler::cmdDebugMode(){
    rootDebugMode(marsRoot, sdCard);
}

void SystemTestHandler::cmdInfo(){
    char *arg;
    arg = getNext();
    if (strcmp(arg, "help") == 0){
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
    else if (strcmp(arg, "all") == 0)
        marsRoot->printDebug();
    else if (strcmp(arg, "root") == 0){
        marsRoot->inError = true;
        marsRoot->printDebug();
        marsRoot->inError = false;
    }
    else if (strcmp(arg, "pressure") == 0)
        prsSensor->printDebug();
    else if (strcmp(arg, "sdcard") == 0)
        sdCard->printDebug();
    else if (strcmp(arg, "stepper") == 0)
        sandwitch->printDebug();
    else if (strcmp(arg, "accellgyro") == 0)
        accellGyro->printDebug();
    else if (strcmp(arg, "fans") == 0)
        fanController->printDebug();
    else if (strcmp(arg, "gps") == 0)
        gpsRadio->printDebug();
    else if (strcmp(arg, "wireless") == 0)
        wirelessModule->printDebug();
    else {
        DBG_FPRINT_SVLN("Unknown Argument: ", arg);
        DBG_FPRINTLN("Use \'info help\' for more infomation.");
    }
}

/*
void SystemTestHandler::cmdSet(){
    char *arg;
    arg = getNext();
    if (strcmp(arg, "help") == 0){
        DBG_FPRINTLN("Available \'set\' arguments are:");
        DBG_FPRINTLN("    fan <options>         [Fan Controller Options]");
        DBG_FPRINTLN("        speed <int>       Set the ESC / EDF speed.");
        DBG_FPRINTLN("        loop              Begin a loop to actually start the EDF\'s.");
        //DBG_FPRINTLN("    pressure <options>    [Pressure Sensor Options]");
        //DBG_FPRINTLN("        ");
        //DBG_FPRINTLN("    sdcard <options>      [SDCard Options]");
        //DBG_FPRINTLN("        ");
        //DBG_FPRINTLN("    stepper <options>     [Stepper Controller Options]");
        //DBG_FPRINTLN("        ");
        //DBG_FPRINTLN("    accellgyro <options>  [Accell/Gyro Sensor Options]");
        //DBG_FPRINTLN("        ");
        //DBG_FPRINTLN("    gps <options>         [GPS Radio Options]");
        //DBG_FPRINTLN("        ");
        //DBG_FPRINTLN("    wireless <options>    [Wireless Module Options]");
        //DBG_FPRINTLN("        ");
    }
    else if (strcmp(arg, "fan") == 0){
        char *arg2;
        arg2 = getNext();
        if (strcmp(arg2, "speed") == 0){
            char *arg3;
            arg3 = getNext();
            if (arg3 == NULL){
                DBG_FPRINT("No speed given!");
                return;
            }
            int newSpeed = atoi(arg3);
            DBG_FPRINT_SVLN("New Speed set to: ", newSpeed);
            sandwitch->write(newSpeed);

        } else if (strcmp(arg2, "loop") == 0){
                bool inLoop = true;
                clearSerial();
                while(inLoop){
                    DBG_FPRINTLN("\e[1;1HFan Speed Loop");
                    DBG_FPRINT("\e[2;1HUse the \'ESC\' or \'e\' key to exit.");
                    while (Serial.available() > 0){
                        char key = (char)Serial.read();
                        if (key == 27 || key == 127 || key == 'e')
                            inLoop = false;
                    }
                    while (!Serial.available())
                        sandwitch->writeSpeed();
                }
        } else
            badArgument()
    }
    else if (strcmp(arg, "stepper") == 0){}
    else if (strcmp(arg, "sdcard") == 0){}
    else if (strcmp(arg, "wireless") == 0){}

    else if (strcmp(arg, "gps") == 0){}
    else if (strcmp(arg, "root") == 0){}
    else if (strcmp(arg, "pressure") == 0){}
    //else if (strcmp(arg, "accellgyro") == 0){}
    
    else {
        DBG_FPRINT_SVLN("Unknown Argument: ", arg);
        DBG_FPRINTLN("Use \'set help\' for more infomation.");
    }
}
*/

//Static Wrappers for the Serial Handler
static void SystemTestHandler::staticUnknownCommand(const char *command){
    ((SystemTestHandler*) gPtrSystemTestHandler)->unknownCommand(command);
}
static void SystemTestHandler::staticCmdHelp(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdHelp();
}
static void SystemTestHandler::staticCmdInfo(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdInfo();
}
static void SystemTestHandler::staticCmdReset(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdReset();
}
static void SystemTestHandler::staticCmdStepper(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdStepper();
}
static void SystemTestHandler::staticCmdFans(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdFans();
}
static void SystemTestHandler::staticCmdLEDs(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdLEDs();
}
static void SystemTestHandler::staticCmdWirelessTest(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdWirelessTest();
}
static void SystemTestHandler::staticCmdDebugMode(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdDebugMode();
}
/*
static void SystemTestHandler::staticCmdSet(){
    ((SystemTestHandler*) gPtrSystemTestHandler)->cmdSet();
}
*/
