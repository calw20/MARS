#include "system_tests.h"

//[TODO] Should generalise Serial to DBG_OUT_OBJ properly
SerialTerminal term('\n', ' ');

void testMode(){
    //Setup
    // Set default handler for unknown commands
    term.setDefaultHandler(unknownCommand);

    // Add command callback handlers
    term.addCommand("?", cmdHelp);
    term.addCommand("help", cmdHelp);
    term.addCommand("stepper", cmdStepper);
    term.addCommand("fans", cmdFans);

    DBG_FPRINTLN("Entering Test Mode.");
    DBG_FPRINTLN("Enter 'help' for useage.");
    while(1){
        term.readSerial();
    }
}

void unknownCommand(const char *command){
    DBG_FPRINTLN("Unknown command: ");
    DBG_PRINTLN(command);
}

//Help Menu
void cmdHelp(){
    DBG_FPRINTLN("Serial terminal usage:");
    DBG_FPRINTLN("  help or ?       Print this usage");
    DBG_FPRINTLN("  stepper <steps> Test Stepper");
    DBG_FPRINTLN("  fans <speed>    Test Fan Controllers / EDFs");
    /*DBG_FPRINTLN("  off                   Turn LED off");
    DBG_FPRINTLN("  hello [name] [age]    Print your name and age");
    DBG_FPRINTLN("  s <arg 1>..<arg N>    Print string arguments");
    DBG_FPRINTLN("  i <arg>               Print decimal or hex argument");*/
}

void cmdStepper(){
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
    stepperTestRotation(sandwitch, val, 1000);
}

void cmdFans(){
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
    fanTest(fanController, val, 5000);
}