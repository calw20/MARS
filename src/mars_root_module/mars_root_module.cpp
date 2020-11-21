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
        cLED1->setColour(LEDColours::RED);   
        delay(100);
        cLED1->setColour(LEDColours::BLACK);
        delay(100);
    }
}

bool RootModule::init(){
    cLED1 = new TriColourLED(LEDPIN_R_1, LEDPIN_G_1, LEDPIN_B_1);
    cLED2 = new TriColourLED(LEDPIN_R_2, LEDPIN_G_2, LEDPIN_B_2);
    data.rotateOnAltitude = ROTATE_ON_ALTITUDE;
    data.rotateOnButton = ROTATE_ON_BUTTON;
    bool baseInit = UnCrashable::init();
    cLED2->setColour(LEDColours::BLACK);
    return baseInit;
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

fStoredData RootModule::selectAltitude(){
    //Cast to fStoredData
    //There is *some* precision loss here but for our application I dont think this is an issue. 
    fStoredData alt;
    alt[0] = data.gpsAltitude[0];
    alt[1] = data.gpsAltitude[1];
     
    return alt;
}

bool RootModule::updatePayloadData(bool forceDataUpdate){
    for (int i = 0; i < (sizeof(modules)/sizeof(modules[0])); i++){
        if (modules[i]){
            modules[i]->updatePayloadData(forceDataUpdate);
        }
    }
    data.altitude = selectAltitude();
    return true;
}

bool RootModule::addModule(MARSCrashableModule &module){
    for (int i = 0; i < (sizeof(modules)/sizeof(modules[0])); i++){
        if (!modules[i]) {
            modules[i] = &module;
            return true;
        }
    }
    return false;
}

bool RootModule::addModule(MARSCrashableModule &module, int id){
    if (id >= sizeof(modules)/sizeof(modules[0]))
        return false;

    modules[id] = &module;
    return true;
}

bool MARSCrashableModule::updatePayloadData(bool forceDataUpdate){
    return true;
}

MARSCrashableModule::MARSCrashableModule(RootModule &uncrashableParent, bool addSelfToParent)
        : CrashableModule((UnCrashable&) uncrashableParent, addSelfToParent) {
    
    marsRoot = &uncrashableParent;
    //pData = &uncrashableParent.data;
    if (addSelfToParent) marsRoot->addModule(*this); //Add itself to the parent.
}