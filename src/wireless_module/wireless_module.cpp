#include "wireless_module.h"

WirelessModule::WirelessModule(RootModule &uncrashableParent, SystemTestHandler *ptrTestHandler, StepperMotor *ptrStepper, bool addSelfToParent) 
    : WirelessRadio(), MARSCrashableModule(uncrashableParent, addSelfToParent) {
    testHandler = ptrTestHandler;
    ptrSandwitch = ptrStepper;    
};

void WirelessModule::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("################## Wireless Radio Error Info ##################");
    DBG_FPRINTF("The Wireless Radio have experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void WirelessModule::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    if (CHK_LETTER("-H")) printValues = "R";

    if (CHK_LETTER("H")) DBG_FPRINTLN("===================== Wireless Radio Info =====================");
    
    if (CHK_LETTER("R")){
        DBG_FPRINTFN("Radio acting in ", "%s mode.", NODE_ADDRESS ? "Rocket" : "Control");
        DBG_FPRINT_SVLN("Radio Address: ", node_addresses[NODE_ADDRESS]);
        DBG_FPRINT_SVLN("Max Buffer Size: ", maxDataBufferSize());
        DBG_FPRINT_SVLN("Radio timeout (ms): ", RADIO_TIMEOUT);
        DBG_FPRINT_SVLN("Radio data resend request timout (ms): ", RESEND_REQUEST_TIMEOUT);
        DBG_FPRINT_SVLN("Wait for resend request? ", WAIT_FOR_RESEND_REQUEST ? F("Yes") : F("No"));
    }
}

//[TODO] Functionise these 🤔🤔
//There is a lot of sending back payloadData on command completion,
//May want to make this recursive
bool WirelessModule::handleWirelessCommand(WirelessCommands cmd, void *buffer){
    bool rVal = true;
    switch (cmd){
    //These *should* never be implimented (ie send a noResponse)
    //NoCommand, ResendData, AcceptSystemReset
    
    case WirelessCommands::SendState: {
        rVal &= sendResponse(WirelessResponses::SystemState, &(marsRoot->data));
    } break;

    case WirelessCommands::ArmPayload: { 
        marsRoot->systemArmed = true; 
    }
    case WirelessCommands::SendArmState: {
        rVal &= sendResponse(WirelessResponses::SystemArmed, &(marsRoot->systemArmed));
    } break;

    case WirelessCommands::ForceFilterRotation: {
        bool val = ptrSandwitch->nextFilter();
        //Not an important value to track but nice to have
        sendResponse(WirelessResponses::CommandSucceeded, &val);
        rVal &= sendResponse(WirelessResponses::SystemState, &(marsRoot->data));
    } break;

    //[TODO] Implement Tests
    case WirelessCommands::PerformSelfTest: {
        //[HACK] This could be an issue - pointer are no fun.
        PerformTest testCMD = *((PerformTest*) dataBuffer);
        //[TODO] Do the tests
        //runTest(testCMD);
        rVal &= sendResponse(WirelessResponses::SystemState, &(marsRoot->data));        
    } break;

    //[TODO] Do reset :/
    case WirelessCommands::ResetSystem: {
        WirelessCommands nCMD = waitForCommand(500);
        if (nCMD == WirelessCommands::AcceptSystemReset){} //Do reset
        unsigned long timeNow = millis();
        rVal &= sendResponse(WirelessResponses::SystemReinitializing, &timeNow);
        
        resetSystem();
    } break;

    default: {
        Serial.print(7);
        rVal &= sendResponse(WirelessResponses::NoResponse);
        Serial.print(8);
    } break;
    }
    return rVal;
}

bool WirelessModule::waitForHandledCommand(unsigned long timeout){
   WirelessCommands cmd = waitForCommand(timeout, dataBuffer);
   Serial.print(5);
   return handleWirelessCommand(cmd, dataBuffer);
}