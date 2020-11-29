/*#include "wireless_module.h"

//Makes things look neater I guess?
bool WirelessModule::init(){
    DBG_FPRINTLN("Initializing Radio");

    radio = new RF24(CE_PIN, CS_PIN);
    radio->begin(); //Init Radio
    radio->setPALevel(RF24_PA_HIGH); //This *may* want to be RF24_PA_LOW
    //Setup read/write pipes
    radio->openWritingPipe(radioPipeNames[0 + RADIOMODE]);
    radio->openReadingPipe(1, radioPipeNames[1 - RADIOMODE]);
    radio->startListening();

    //Send Init Code
    radio->stopListening();
    WirelessResponses sendCode = SystemInitialized;
    unsigned long cTime = millis();
    radio->write(&sendCode, sizeof(sendCode));
    radio->write(&cTime, sizeof(cTime));
    radio->startListening();

    DBG_FPRINT("Initialized Radio");

    return true;
}

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

bool WirelessModule::waitForData(){
    unsigned long got_time;
    WirelessCommands got_command;

    if( radio->available()){
    while (radio->available()) {                                   // While there is data ready
      radio->read( &got_command, sizeof(WirelessCommands) );             // Get the payload
    }                                                            // Variable for the received timestamp
    while (radio->available()) {                                   // While there is data ready
      radio->read( &got_time, sizeof(unsigned long) );             // Get the payload
    }

    radio->stopListening();                                        // First, stop listening so we can talk   
    //radio->write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
    radio->startListening();                                       // Now, resume listening so we catch the next packets.     
    printfn(Serial, "%lu,%lu", millis(), got_time);
  }
}*/