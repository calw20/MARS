#ifndef __MARS_GPS_H__
#define __MARS_GPS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

//TinyGPS++ Is an included lib, see ../TinyGPSPlus/README.md for more info
#include "../TinyGPSPlus/src/TinyGPS++.h"
//#include <SoftwareSerial.h>

//#define GPS_RX 19
//#define GPS_TX 18
#define GPS_BAUD 9600
#define GPS_TIMEOUT 500 //milliSeconds
#define GPS_INIT_TIMEOUT 5*60*10*100UL //milliSeconds [Note the UL for forced typeing]

#ifndef USE_GPS
    #define USE_GPS true
#endif

#ifndef ALT_GPS_SAMPLE_COUNT
    #define ALT_GPS_SAMPLE_COUNT 10
#endif

#ifndef MAX_HEIGHT_DIFF_FOR_GND_LVL
    #define MAX_HEIGHT_DIFF_FOR_GND_LVL 2UL //Note the UL
#endif

#ifndef GPS_GEN_TIMEOUT
    #define GPS_GEN_TIMEOUT 5*10*100UL //Note the UL
#endif

class GPSModule : public MARSCrashableModule {
    public:
        GPSModule(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : /*ss(GPS_RX, GPS_TX),*/ MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "G") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        bool updatePayloadData(bool forceDataUpdate) override;

    public:
        bool fetchGPSData();
        TinyGPSAltitude getGPSAltitude();
        TinyGPSLocation getGPSLocation();
        TinyGPSTime getGPSTime();
        TinyGPSPlus getGPS();

    public:
        double getAltitude();
        uint32_t getTime();
        uint32_t getDate();
        dV2d getLatLong();
        double genAltitudeOffset();

    private:
        TinyGPSPlus gps;
        //SoftwareSerial ss;

};
#endif