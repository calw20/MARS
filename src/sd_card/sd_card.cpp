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
        CRITICAL_FAIL();
    }
    DBG_FPRINT_SVLN("Successfully opened file: ", logFileName);
    return LocalFile;
};


bool SDCardAdapter::init(){
    DBG_FPRINTLN("Initalizing SD Card...");
    if (!sdCard.begin(SD_CS_PIN)) {
        DBG_FPRINTLN("Could not find a valid SD Card, check wiring!");
        CRITICAL_FAIL();
    }
    DBG_FPRINTLN("SD Card Initialized.");
   
    return true;
}

bool SDCardAdapter::initFlightDataFile(PressureSensor &prsSensorModule, 
    StepperMotor &sandwitchMotorModule, AccellGyro &accellGyroModule){
    
    prsSensor = &prsSensorModule;
    stpMotor = &sandwitchMotorModule;
    acGyro = &accellGyroModule;
    
    //Attempt to open the file
    SET_LOG_FILE_NUM(0);
    FlightDataFile = openFile(logFileName);

    //if (ROLL_TO_NEXT_FILE) nextFile(sdCard, FlightDataFile, currentFileCount, true);

    //Get the accell/gyro data
    acGyro->getMotion();

    //Print all the inital values to the board
    FlightDataFile.println("Millis, Pressure, Altitude, Ax, Ay, Az, Gx, Gy, Gz, Temperature, Current Filter");
    printfn(FlightDataFile, "%lu,%f,%f,%hi,%hi,%hi,%hi,%hi,%hi,%f,%f", 
        millis(), prsSensor->readPressure(), prsSensor->readAltitude(),
        acGyro->ax, acGyro->ay, acGyro->az,
        acGyro->gx, acGyro->gy, acGyro->gz,
        prsSensor->readTemperature(), prsSensor->getSeaLevelPressure()
    );

    FlightDataFile.flush();
    
    DBG_FPRINTLN("Flight Data File Initialized.");
    
    return true;
}

int SDCardAdapter::genCSVData(bool doUpdate){    
    return sprintf(fmtedFlightData, /*sizeof(fmtFlightData),*/
        "%lu,%f,%f,%hi,%hi,%hi,%hi,%hi,%hi,%f,%f", 
        millis(), prsSensor->pressure[0], prsSensor->altitude[0],
        acGyro->ax, acGyro->ay, acGyro->az,
        acGyro->gx, acGyro->gy, acGyro->gz,
        prsSensor->readTemperature(), stpMotor->getCurrentFilter()
    );
}

void SDCardAdapter::printCSVData(bool doUpdate){
    //acGyro->getMotion();
    /*
    printfn(Serial, "%lu,%f,%f,%hi,%hi,%hi,%hi,%hi,%hi,%f,%f", 
        millis(), prsSensor->pressure[0], prsSensor->altitude[0],
        acGyro->ax, acGyro->ay, acGyro->az,
        acGyro->gx, acGyro->gy, acGyro->gz,
        prsSensor->readTemperature(), stpMotor->getCurrentFilter()
    );*/

    if (doUpdate) genCSVData();
    Serial.println(fmtedFlightData);
}

void SDCardAdapter::writeCSVData(){
    //! Nope we are just going to print the data //Get the accell/gyro data
    //acGyro->getMotion();

    printfn(FlightDataFile, "%lu,%f,%f,%hi,%hi,%hi,%hi,%hi,%hi,%f,%f", 
        millis(),prsSensor->pressure[1], prsSensor->altitude[0],
        acGyro->ax, acGyro->ay, acGyro->az,
        acGyro->gx, acGyro->gy, acGyro->gz,
        prsSensor->readTemperature(), stpMotor->getCurrentFilter()
    );
    
    //Only force a write to the card every FORCE_WRITE_CYCLES cycles
    //This is to ensure we actually have the data
    writeCycle++;
    if (writeCycle >= FORCE_WRITE_CYCLES){
        writeCycle = 0;
        FlightDataFile.flush();
    }
}
