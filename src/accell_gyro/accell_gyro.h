#ifndef __MARS_ACCELL_GYRO_H__
#define __MARS_ACCELL_GYRO_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"

#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>

#ifndef USE_ACCELL
    #define USE_ACCELL true
#endif


class AccellGyro : public CrashableModule {
    public:
        AccellGyro(UnCrashable &uncrashableParent, bool addSelfToParent = true) 
            : CrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        void printDebug(String = "AG") override;

    public:
        void getMotion();

    public:
        int16_t ax, ay, az; //Accelerometer Values
        int16_t gx, gy, gz; //Gyro Values

        /*
        //[TODO] Get the real values of theses
        //? Calibration Factors
        const float mod_offset = 1670.13252568;
        const f3dv aOff = {110.0, 765.0, (0.0 * mod_offset)}
        const f3dv gOff = {80.0, -11.0, -13.0}
        const int xGyro = 80, yGyro = -11, zGyro = -13;
        */

    protected:
        MPU6050 AccellGyroSensor;
        
        
};

#endif