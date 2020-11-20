#ifndef __MARS_ACCELL_GYRO_H__
#define __MARS_ACCELL_GYRO_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>

#ifndef USE_ACCELL
    #define USE_ACCELL true
#endif


class AccellGyro : public MARSCrashableModule {
    public:
        AccellGyro(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        void printDebug(String = "AG") override;
        bool updatePayloadData(bool forceDataUpdate) override;

    public:
        void getMotion();

    public:
        i16V3d a; //Accelerometer Values
        i16V3d g; //Gyro Values

        int16_t ax, ay, az; //Accelerometer Values
        int16_t gx, gy, gz; //Gyro Values

        //[TODO] Make these Radio Receivable
        //[TODO] Get the real values of theses
        //? Calibration Factors
        float mod_offset = 1670.13252568;
        fV3d aOff = {110.0, 765.0, (0.0 * mod_offset)};
        fV3d gOff = {80.0, -11.0, -13.0};
        int xGyro = 80, yGyro = -11, zGyro = -13;

    protected:
        MPU6050 AccellGyroSensor;
        
        
};

#endif