#include "accell_gyro.h"

bool AccellGyro::init(){
    DBG_FPRINTLN("Initalizing Accell/Gyro...");

    DBG_FPRINTLN("Joining I2C Bus...");
    Wire.begin();
    DBG_FPRINTLN("I2C Bus active.");

    DBG_FPRINTLN("Attemping to start Accell/Gyro Hardware...");
    AccellGyroSensor.initialize();
    #if USE_ACCELL
        DBG_FPRINTLN("Hardware started.");
    #else
        DBG_FPRINTLN("Hardware disabled in settings!");
    #endif
    
    if (!AccellGyroSensor.testConnection()) {
        DBG_FPRINTLN("Could not connect to Accell/Gyro, check wiring!");
        
        #if USE_ACCELL
            CRITICAL_FAIL();
        #else
            minorFailure(__FUNCTION__, __FILE__, __LINE__);
        #endif
    }
    DBG_FPRINTLN("Accell/Gyro Initialized.");
   
    //[TODO] Get the correct values for this
    DBG_PRINT(AccellGyroSensor.getXAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZAccelOffset()); DBG_PRINTLN();
    DBG_PRINT(AccellGyroSensor.getXGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZGyroOffset());  DBG_PRINTLN();
    
    AccellGyroSensor.setXGyroOffset(gOff.x);
        AccellGyroSensor.setYGyroOffset(gOff.y);
        AccellGyroSensor.setZGyroOffset(gOff.z);
    AccellGyroSensor.setXAccelOffset(aOff.x);
        AccellGyroSensor.setYAccelOffset(aOff.y);
        AccellGyroSensor.setZAccelOffset(aOff.z);
    
    DBG_PRINT(AccellGyroSensor.getXAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZAccelOffset()); DBG_PRINTLN();
    DBG_PRINT(AccellGyroSensor.getXGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZGyroOffset());  DBG_PRINTLN();
    

    return true;
}

void AccellGyro::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("#################### Accell/Gyro Error Info ####################");
    DBG_FPRINTF("The Accelleration / Gyroscopic motion Sensor has experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void AccellGyro::printDebug(String printValues){
    if (CHK_LETTER("-H")) printValues = "Ag";

    if (CHK_LETTER("H")) DBG_FPRINTLN("======================= Accell/Gyro Info =======================");
    
    //*Print Accel Info
    if (CHK_LETTER("a")){
        DBG_PRINT(F("Accelerometer X, Y, Z: "));
            DBG_PRINT(a.x); DBG_PRINT(F(", "));
            DBG_PRINT(a.y); DBG_PRINT(F(", "));
            DBG_PRINTLN(a.z);
    }

    //*Print Gyro Info
    if (CHK_LETTER("g")){
        DBG_PRINT(F("Gyro X, Y, Z: "));
            DBG_PRINT(g.x); DBG_PRINT(F(", "));
            DBG_PRINT(g.y); DBG_PRINT(F(", "));
            DBG_PRINTLN(g.z);
    }
}

void AccellGyro::getMotion(){
    AccellGyroSensor.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
}

bool AccellGyro::updatePayloadData(bool forceDataUpdate){
    if (forceDataUpdate) getMotion();
    marsRoot->data.a = a;
    marsRoot->data.g = g;
    return true;
};