#include "sd_card.h"

//Generic Boilerplate Stuff
void SDCardAdapter::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("###################### SD Card Error Info ######################");
    DBG_FPRINTF("The SD Card has experienced an Error of type: ","[%s]\r\nMore Details;\r\n", 
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
};

void SDCardAdapter::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1

    if (CHK_LETTER("-H")) printValues = "S";

    if (CHK_LETTER("H")) DBG_FPRINTLN("========================= SD Card Info =========================");
    if (CHK_LETTER("S")){
        DBG_FPRINT_SVLN("Curent File Open: ", logFileName);
        //[TODO] Add all the card / file info
    }
}


//Open a new file and return it or Hard Fail if it cant open one
File SDCardAdapter::openFile(char* logFileName, bool delFileOnOpen){
    if (delFileOnOpen) sdCard.remove(logFileName);
    DBG_FPRINT_SVLN("Opening file: ", logFileName);
    File LocalFile = sdCard.open(logFileName, O_WRITE | O_CREAT);
    if (!LocalFile) {
        DBG_FPRINT_SVLN("Could not open file: ", logFileName);
        #if USE_SD_CARD
            CRITICAL_FAIL();
        #else
             DBG_FPRINTLN("Hardware disabled in settings!");
            minorFailure(__FUNCTION__, __FILE__, __LINE__);
        #endif
    }
    DBG_FPRINT_SVLN("Successfully opened file: ", logFileName);
    return LocalFile;
};


bool SDCardAdapter::closeFile(){
    if(FlightDataFile){
        FlightDataFile.flush();
        return FlightDataFile.close();
    } else {
        return false;
    }
}

bool SDCardAdapter::init(){
    DBG_FPRINTLN("Initalizing SD Card...");
    if (!sdCard.begin(SD_CS_PIN) && USE_SD_CARD) {
        DBG_FPRINTLN("Could not find a valid SD Card, check wiring!");
        CRITICAL_FAIL();
    } else if (!USE_SD_CARD){
        DBG_FPRINTLN("Hardware disabled in settings!");
        minorFailure(__FUNCTION__, __FILE__, __LINE__);
    }
    DBG_FPRINTLN("SD Card Initialized.");
   
    return true;
}

bool SDCardAdapter::initFlightDataFile(){    
    //Attempt to open the file
    SET_LOG_FILE_NUM(0);
    FlightDataFile = openFile(logFileName);

    //Print all the inital values to the board
    FlightDataFile.println("UTC Timestamp,Altitude,Current Filter,Temperature,Latitude,Longitude,Millis,Pressure,Ax,Ay,Az,Gx,Gy,Gz"); //Print Header
    genCSVData(true, true); //Force write base values

    FlightDataFile.flush();
    
    DBG_FPRINTLN("Flight Data File Initialized.");
    
    return true;
}

int SDCardAdapter::genCSVData(bool pollNewData, bool forceDataUpdate){
    if (pollNewData) marsRoot->updatePayloadData(forceDataUpdate);
    
    payloadData &pData = marsRoot->data;
    return sprintf(fmtedFlightData, "%lu,%f,%u,%f,%lu,%lu,%lu,%hi,%hi,%hi,%hi,%hi,%hi",
        pData.time, pData.altitude[0], pData.currentFilter, pData.temp[0],
        (unsigned long)pData.position[0], (unsigned long)pData.position[1],
        millis(), pData.pressure[0],
        pData.a.x, pData.a.y, pData.a.z,
        pData.g.x, pData.g.y, pData.g.z, 
    );
}

/*
int SDCardAdapter::genCSVData(bool pollNewData) {
    return genCSVData(pollNewData, false);
}*/

void SDCardAdapter::printCSVData(bool doUpdate, bool forceDataUpdate){
    genCSVData(doUpdate, forceDataUpdate);
    Serial.println(fmtedFlightData);
}

void SDCardAdapter::writeCSVData(){
    genCSVData(false);
    FlightDataFile.println(fmtedFlightData);

    //Only force a write to the card every FORCE_WRITE_CYCLES cycles
    //This is to ensure we actually have the data
    writeCycle++;
    if (writeCycle >= FORCE_WRITE_CYCLES){
        writeCycle = 0;
        FlightDataFile.flush();
    }
}
