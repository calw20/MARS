#ifndef __MARS_BMP280_H__
#define __MARS_BMP280_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include <Wire.h>
#include <Adafruit_BMP280.h>

#ifndef USE_PRESSURE_SENSOR
    #define USE_PRESSURE_SENSOR true
#endif

#ifndef ALT_SAMPLE_COUNT
    #define ALT_SAMPLE_COUNT 10
#endif

#ifndef MIN_PRESS_DIFF_FOR_SEA_LVL
    #define MIN_PRESS_DIFF_FOR_SEA_LVL 5
#endif

//Use the ground as 0m if a SEA_LEVEL_PRESSURE is not given
#ifndef SEA_LEVEL_PRESSURE
    #define SEA_LEVEL_PRESSURE genSeaLevelPressure()
#endif

class PressureSensor : public MARSCrashableModule {
    public:
        PressureSensor(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "B") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        bool updatePayloadData(bool forceDataUpdate) override;

    public:
        float genSeaLevelPressure();
        float getSeaLevelPressure();

    public:
        float readPressure();
        float readAltitude();
        float readTemperature();

    public:
        void updateTempAltPressure();
            void updateAltPressure();
                float getPressure();
                float getAltitude();
            void updateTemp();
                float getTemp();

    private:
        Adafruit_BMP280 BMP280;
        float sea_level_pressure;
        
        fStoredData pressure;            
        fStoredData altitude;
        fStoredData temp;
        
};
#endif