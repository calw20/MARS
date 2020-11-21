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

//Initialize the GPS Radio
bool GPSModule::init(){
    DBG_FPRINTLN("Initalizing GPS...");
    #if USE_GPS
        if (!fetchGPSData()) {
            DBG_FPRINTLN("Failed detection on first check, attempting again...");
            if (!fetchGPSData()){
                DBG_FPRINTLN("Could not find a valid GPS Radio, check wiring!");
                CRITICAL_FAIL();
            }
        }
        DBG_FPRINTLN("GPS Initialized.");
        
    #else
        DBG_FPRINTLN("Hardware disabled in settings!");
        minorFailure(__FUNCTION__, __FILE__, __LINE__);
    #endif
    
    return true;
};

bool GPSModule::fetchGPSData(){
    //[TODO] See if this timeout could be better
    unsigned long startTime = millis();
    while (ss.available() > 0)
        gps.encode(ss.read());

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

TinyGPSAltitude GPSModule::getAltitude(){ return gps.altitude; }
TinyGPSLocation GPSModule::getLocation(){ return gps.location; }
TinyGPSTime GPSModule::getTime(){ return gps.time; }
TinyGPSPlus GPSModule::getGPS(){ return gps; }

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

bool GPSModule::updatePayloadData(bool forceDataUpdate){
    if (forceDataUpdate && USE_GPS) fetchGPSData();
    marsRoot->data.gpsAltitude = gps.altitude.meters();
    marsRoot->data.time = gps.time.value();
    marsRoot->data.position[0] = gps.location.lat();
    marsRoot->data.position[1] = gps.location.lng();

   return true;
}


