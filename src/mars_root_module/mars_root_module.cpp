#include "mars_root_module.h"

void RootModule::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("======================= Begin Debug Dump =======================");

    bool inErrorStore = inError;
    inError = true;
    printDebug();
    inError = inErrorStore;
    
    UnCrashable::genericError(func, file, failLine);
    DBG_FPRINTLN("======================== End Debug Dump ========================");
}

void RootModule::criticalFailure(const char* func, const char* file, u16 failLine){
    inError = true;
    status = CrashType::Critical;

    genericError(func, file, failLine);

    while(inError){
        digitalWrite(CRITICAL_LED, HIGH);   
        delay(100);
        digitalWrite(CRITICAL_LED, LOW);   
        delay(100);
    }
}

void RootModule::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    
    //Look if a blank string is passed in just ignore it and print a newline
    if (printValues == ""){
        DBG_PRINTLN();
        return;
    }

    if (CHK_LETTER("-H")) printValues = "NHTB";

    //Header
    if (CHK_LETTER("NH")) DBG_FPRINTLN("====================== M.A.R.S Debug Info ======================");
    if (CHK_LETTER("H")) {
        DBG_FPRINTFN("Build Date: ", "%s", BUILD_DATE);
        DBG_FPRINTFN("Build Version: ", "%s", BUILD_VERSION);
        if (CHK_LETTER("+H")) DBG_FPRINTFN("Given String: ", "%s", printValues);
    }

    //Time
    if (CHK_LETTER("TM"))
        DBG_FPRINTFN("System Uptime: ", "%f miniutes.", millis()/60000.00);
    else if (CHK_LETTER("T"))
        DBG_FPRINTFN("System Uptime: ", "%f seconds.", millis()/1000.00);

    //Print all the other info ONLY if not in an error
    if (!inError)
        UnCrashable::printDebug(printValues);
    

}

bool MARSCrashableModule::updatePayloadData(bool forceDataUpdate){};