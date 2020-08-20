#include "pressure_senor.h"

//Generic Boilerplate Stuff
void PressureSensor::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("################## Pressure Sensor Error Info ##################");
    DBG_FPRINTF("The Pressure Sensor has experienced an Error of type: ","[%s]\r\nMore Details;\r\n", 
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
};

//Map BMP280 Functions to our class
float PressureSensor::readPressure(){
    return BMP280.readPressure();
}

//We have a class, why keep passing semi-const values?
float PressureSensor::readAltitude(){
    return BMP280.readAltitude(sea_level_pressure);
} 

float PressureSensor::readTemperature() {
    return BMP280.readTemperature();
}

void PressureSensor::updateAltPressure(){
    //pressure.previous = pressure.current;
    //pressure.current = readPressure();
    
    //altitude.previous = altitude.current;
    //altitude.current = readAltitude();

    pressure = readPressure();
    altitude = readAltitude();

}

float PressureSensor::getPressure(){
    updateAltPressure();
    return pressure.current;
}

float PressureSensor::getAltitude(){
    updateAltPressure();
    return altitude.current;
}

float PressureSensor::getSeaLevelPressure() {
    return sea_level_pressure;
}


//Now the fun values.
float PressureSensor::genSeaLevelPressure(){
    DBG_FPRINTLN("Generating Sea Level Pressure.");
    
    int minDiff = 5;
    int inLoop = 0;
    float pressure1, pressure2, avgPressure;
    bool inRange;
    while (inLoop < ALT_SAMPLE_COUNT) {
        pressure1 = readPressure();
        delay(300);
        pressure2 = readPressure();
        inRange = abs(pressure1 - pressure2) < minDiff;
        
        if (inRange) avgPressure += pressure2; //If within range then add it to the avg pressure
        inLoop += (int)inRange;

        DBG_PRINTFN("Attempt %d: %F - %F = %F < %d? %s", inLoop, 
            pressure1, pressure2, pressure1-pressure2, minDiff, inRange ? "Yes!" : "No.");
    }
    
    //[TODO] Create better average, when using the above it seems to be off by ~ 700m
    avgPressure = pressure2;// avgPressure/(float)ALT_SAMPLE_COUNT; //Calculate the average pressure for the location

    DBG_FPRINTFN("Sea Level Pressure is set to: ", "%.3fhPa", avgPressure*0.01);

    return avgPressure*0.01; //Return the average pressure in hPa
}

//Initialize the BMP280 Pressure Sensor
bool PressureSensor::init(){
    //Cry, I mean error out if the BMP280 is not initalized
    DBG_FPRINTLN("Initalizing BMP280...");
    #if USE_PRESSURE_SENSOR
        if (!BMP280.begin()) {
            DBG_FPRINTLN("Could not find a valid BMP280 sensor, check wiring!");
            CRITICAL_FAIL();
        }
        DBG_FPRINTLN("BMP280 Initialized.");
        
        //Setup the BMP280
        //Default settings from datasheet
        DBG_FPRINTLN("Configuring BMP280");
        BMP280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        DBG_FPRINTLN("Configured BMP280");
        sea_level_pressure = SEA_LEVEL_PRESSURE;
    #else
        sea_level_pressure = 1;
        DBG_FPRINTLN("Hardware disabled in settings!");
        minorFailure(__FUNCTION__, __FILE__, __LINE__);
    #endif


    
    DBG_FPRINTFN("Sea Level Pressure set to: ", "%.3f", sea_level_pressure);
    return true;
}

//BMP280 Debug Output
void PressureSensor::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1

    if (CHK_LETTER("-H")) printValues = "B";
        
    if (CHK_LETTER("H")) DBG_FPRINTLN("===================== Pressure Sensor Info =====================");
    
    DBG_FPRINT("fhgkhjgkfjhgkjh");

    //BMP280
    bool bmpAvailable = (USE_PRESSURE_SENSOR && BMP280.getStatus() != 255); //Check that the BMP280 is connected and initialized
    DBG_FPRINT("fhgkhjgkfjhgkjh");

    //Sensor Catch
    if (!bmpAvailable && (CHK_LETTER("B") || CHK_LETTER("P") || CHK_LETTER("A") || CHK_LETTER("D")))
        DBG_FPRINTLN("BMP280 is not detected. Check wiring!");
    //Altitude
    if (bmpAvailable && (CHK_LETTER("A") || CHK_LETTER("B"))) {
        DBG_FPRINTFN("Approximate Altitude: ", "%.3fm", readAltitude());
        DBG_FPRINTFN("Pressure at Sea Level: ", "%.3fhPa", sea_level_pressure);
        DBG_FPRINTFN("Pressure (hPa): ", "%.3fhpa", readPressure()/100);
        DBG_FPRINTFN("Current (Stored) Altitude (hPa): ", "%.3fm", altitude.current);
        DBG_FPRINTFN("Previous (Stored) Altitude (hPa): ", "%.3fm", altitude.previous);
    }
    //Pressure
    if (bmpAvailable && (CHK_LETTER("P") || CHK_LETTER("B")))
        DBG_FPRINTFN("Pressure (Pa): ", "%.3fPa", readPressure());
        DBG_FPRINTFN("Current (Stored) Pressure (hPa): ", "%.3fhpa", pressure.current/100);
        DBG_FPRINTFN("Previous (Stored) Pressure (hPa): ", "%.3fhpa", pressure.previous/100);
    //Temp
    if (bmpAvailable && (CHK_LETTER("D") || CHK_LETTER("B")))
        DBG_FPRINTFN("Temperature: ", "%.3f*C", readTemperature()); //None of this FreedomHeight Nonsense
}


