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
   
    /*//[TODO] Actually enable this
    DBG_PRINT(AccellGyroSensor.getXAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZAccelOffset()); DBG_PRINTLN();
    DBG_PRINT(AccellGyroSensor.getXGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZGyroOffset());  DBG_PRINTLN();
    
    AccellGyroSensor.setXGyroOffset(xOffset);
        AccellGyroSensor.setYGyroOffset(yOffset);
        AccellGyroSensor.setZGyroOffset(zOffset);
    AccellGyroSensor.setXAccelOffset(xOffset);
        AccellGyroSensor.setYAccelOffset(yOffset);
        AccellGyroSensor.setZAccelOffset(zOffset);
    
    DBG_PRINT(AccellGyroSensor.getXAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYAccelOffset()); DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZAccelOffset()); DBG_PRINTLN();
    DBG_PRINT(AccellGyroSensor.getXGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getYGyroOffset());  DBG_PRINT(", ");
        DBG_PRINT(AccellGyroSensor.getZGyroOffset());  DBG_PRINTLN();
    */

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
    if (CHK_LETTER("-H")) printValues = "AG";

    if (CHK_LETTER("H")) DBG_FPRINTLN("======================= Accell/Gyro Info =======================");
    
    //*Print Accel Info
    if (CHK_LETTER("A")){
        DBG_PRINT(F("Accelerometer X, Y, Z: "));
            DBG_PRINT(ax); DBG_PRINT(F(", "));
            DBG_PRINT(ay); DBG_PRINT(F(", "));
            DBG_PRINTLN(az);
    }

    //*Print Gyro Info
    if (CHK_LETTER("G")){
        DBG_PRINT(F("Gyro X, Y, Z: "));
            DBG_PRINT(gx); DBG_PRINT(F(", "));
            DBG_PRINT(gy); DBG_PRINT(F(", "));
            DBG_PRINTLN(gz);
    }
}


void AccellGyro::getMotion(){
    AccellGyroSensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}