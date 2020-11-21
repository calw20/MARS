#include "gps.h"

//Generic Boilerplate Stuff
void GPSModule::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("##################### GPS Radio Error Info #####################");
    DBG_FPRINTF("The GPS Radio has experienced an Error of type: ","[%s]\r\nMore Details;\r\n", 
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
};

//GPS Debug Output
void GPSModule::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1

    if (CHK_LETTER("-H")) printValues = "G";
    
    if (CHK_LETTER("H")) DBG_FPRINTLN("======================== GPS Radio Info ========================");
    if (CHK_LETTER("H")) {
        DBG_FPRINT_SVLN("TinyGPS++ Version: v", TinyGPSPlus::libraryVersion());
    }

    //GPS Data
    if ((CHK_LETTER("L") || CHK_LETTER("G"))){
        DBG_FPRINT_SVLN("Location Fix Age: ", gps.location.age());
        DBG_FPRINT_SVLN("Raw Lat: ", gps.location.lat());
        DBG_FPRINT_SVLN("Raw Long: ", gps.location.lng());
        
        DBG_FPRINT_SVLN("Altitude Age: ", gps.altitude.age());
        DBG_FPRINT_SVLN("Altitude Raw [km]: ", gps.altitude.kilometers());
        DBG_FPRINT_SVLN("Altitude Raw [m]: ", gps.altitude.meters());
        DBG_FPRINT_SVLN("Altitude Raw [ms]: ", gps.altitude.value());
    }
    
    if ((CHK_LETTER("S") || CHK_LETTER("G"))){
        DBG_FPRINT_SVLN("SATELLITES Fix Age: ", gps.satellites.age());
        DBG_FPRINT_SVLN("SATELLITES ms Value: ", gps.satellites.value());

        DBG_FPRINT_SVLN("HDOP Fix Age: ", gps.hdop.age());
        DBG_FPRINT_SVLN("HDOP: ", gps.hdop.value());
        DBG_FPRINT_SVLN("HDOP ms raw: ", gps.hdop.hdop());
    }

    if ((CHK_LETTER("t") || CHK_LETTER("G"))){
        DBG_FPRINT_SVLN("GPS Time Age: ", gps.time.age());
        DBG_FPRINT("GPS Time: ");
        //Hour
        if (gps.time.hour() < 10) DBG_FPRINT("0");
        DBG_PRINT(gps.time.hour());
        //Minute
        DBG_PRINT(":");
        if (gps.time.minute() < 10) DBG_FPRINT("0");
        DBG_PRINT(gps.time.minute());
        //Seconds
        DBG_FPRINT(":");
        if (gps.time.second() < 10) DBG_FPRINT("0");
        DBG_PRINT(gps.time.second());
        //Centi-Seconds
        DBG_FPRINT(".");
        if (gps.time.centisecond() < 10) DBG_FPRINT("0");
        DBG_PRINTLN(gps.time.centisecond());
    }

    if ((CHK_LETTER("d") || CHK_LETTER("G"))){
        DBG_FPRINT_SVLN("GPS Date Age: ", gps.time.age());
        DBG_FPRINT("GPS Date: ");
        DBG_PRINT(gps.date.day()); DBG_FPRINT("/");
        DBG_PRINT(gps.date.month()); DBG_FPRINT("/");
        DBG_PRINTLN(gps.date.year());
    }
}

//Initialize the GPS Radio
bool GPSModule::init(){
    DBG_FPRINTLN("Initalizing GPS...");
    #if USE_GPS
        Serial1.begin(GPS_BAUD);
        DBG_FPRINTLN("Waiting for GPS Lock...");
        
        unsigned long initTime = millis();
        unsigned long endTime = initTime;

        while (gps.satellites.value() < 1 && millis()-initTime < GPS_INIT_TIMEOUT){
            fetchGPSData();
            endTime = millis();
        }
        
        if (fetchGPSData()){
                DBG_FPRINTFN("GPS Lock Found! ", "%lu Satellites found in %f seconds.", (unsigned long)gps.satellites.value(), endTime/1000.00);
        } else if (endTime-initTime > GPS_INIT_TIMEOUT) {
                DBG_FPRINTFN("No GPS Lock found in ", "%f seconds.", endTime/1000.00);
        } else {
                DBG_FPRINTLN("Could not find a valid GPS Radio, check wiring!");
                CRITICAL_FAIL();
        }
        
        marsRoot->data.altGndLvlOffset = genAltitudeOffset();

        DBG_FPRINTLN("GPS Initialized.");
        
    #else
        DBG_FPRINTLN("Hardware disabled in settings!");
        minorFailure(__FUNCTION__, __FILE__, __LINE__);
    #endif
    
    return true;
};

//Get an update from the module
bool GPSModule::fetchGPSData(){
    //[TODO] See if this timeout could be better
    unsigned long startTime = millis();
    while (Serial1.available() > 0)
        gps.encode(Serial1.read());

    //[TODO] See if this is a valid timeout method....
    if (millis()-startTime > GPS_TIMEOUT && gps.charsProcessed() < 10)
        return false;
    
    /*//[TODO] This may not be compilent to standard.... :/ 
    //Fail if we didn't get a valid location & time
    if (gps.location.isValid() && gps.time.isValid())
        return false;
    */
    return true;
}

//If wanted return compontes
TinyGPSAltitude GPSModule::getGPSAltitude(){ return gps.altitude; }
TinyGPSLocation GPSModule::getGPSLocation(){ return gps.location; }
TinyGPSTime GPSModule::getGPSTime(){ return gps.time; }
TinyGPSPlus GPSModule::getGPS(){ return gps; }

//Return the raw values
double GPSModule::getAltitude(){ return gps.altitude.meters(); }
uint32_t GPSModule::getTime(){ return gps.time.value(); }

dV2d GPSModule::getLatLong(){
    dV2d pos;
    pos[0] = gps.location.lat();
    pos[1] = gps.location.lng();

    return pos;
}

double GPSModule::genAltitudeOffset(){
    DBG_FPRINTLN("Generating ground level altitude offset.");
    
    int inLoop = 0;
    double alt1, alt2, avgAlt;
    bool inHeightRange = false;
    while (inLoop < ALT_GPS_SAMPLE_COUNT) {
        unsigned long startTime = millis();
        fetchGPSData(); alt1 = getAltitude();
        while(!gps.altitude.isUpdated() && millis()-startTime < GPS_GEN_TIMEOUT) fetchGPSData();
        fetchGPSData(); alt2 = getAltitude();
        inHeightRange = abs(alt1 - alt2) < MAX_HEIGHT_DIFF_FOR_GND_LVL;
        
        if (inHeightRange) avgAlt += alt2;
        inLoop += (int)inHeightRange;

        DBG_PRINTFN("Attempt %d: %f - %f = %f < %dm? %s", inLoop, 
            alt1, alt2, alt1-alt2, MAX_HEIGHT_DIFF_FOR_GND_LVL, inHeightRange ? "Yes!" : "No.");
    }
    
    avgAlt = avgAlt/(double)ALT_GPS_SAMPLE_COUNT;

    DBG_FPRINTFN("Ground Level Altitude Offset is set to: ", "%.3fm", avgAlt);

    return avgAlt;
}

bool GPSModule::updatePayloadData(bool forceDataUpdate){
    if (forceDataUpdate && USE_GPS) fetchGPSData();
    marsRoot->data.gpsAltitude = getAltitude();
    marsRoot->data.time = getTime();
    marsRoot->data.position = getLatLong();

   return true;
}


