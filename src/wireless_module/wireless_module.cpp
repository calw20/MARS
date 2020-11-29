#include "wireless_module.h"

void WirelessModule::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("################## Wireless Radio Error Info ##################");
    DBG_FPRINTF("The Fan Controllers have experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void WirelessModule::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    if (CHK_LETTER("-H")) printValues = "R";

    if (CHK_LETTER("H")) DBG_FPRINTLN("===================== Fan Controllers Info =====================");
    
    if (CHK_LETTER("R")){
        //DBG_FPRINT_SVLN("Current Fan Speed: ", currentSpeed);
        
    }
}

//[TODO] Functionise these 🤔🤔 
bool WirelessModule::handleWirelessCommand(WirelessCommands cmd, void *buffer){
    bool rVal = true;
    switch (cmd){
    //These *should* never be implimented (ie send a noResponse)
    //case WirelessCommands::NoCommand:
    //case WirelessCommands::ResendData:
    //case WirelessCommands::AcceptSystemReset:
    
    case WirelessCommands::ArmPayload:
        marsRoot->systemArmed = true;
    case WirelessCommands::SendArmState:
        rVal &= sendResponse(WirelessResponses::SystemArmed, &(marsRoot->systemArmed));
        break;

    case WirelessCommands::ForceFilterRotation:
        //[TODO] Rotate System
    case WirelessCommands::SendState:
        rVal &= sendResponse(WirelessResponses::SystemState, &(marsRoot->data));
        break;
    
    case WirelessCommands::ResetSystem:
        WirelessCommands nCMD = waitForCommand(500);
        if (nCMD == WirelessCommands::AcceptSystemReset){} //Do reset
        unsigned long timeNow = millis();
        rVal &= sendResponse(WirelessResponses::SystemReinitialized, &timeNow);
        break;
        
    default:
        rVal &= sendResponse(WirelessResponses::NoResponse);
        break;
    }
    return rVal;
}

bool WirelessModule::waitForHandledCommand(unsigned long timeout){
   WirelessCommands cmd = waitForCommand(dataBuffer, timeout);
   return handleWirelessCommand(cmd, dataBuffer);
}