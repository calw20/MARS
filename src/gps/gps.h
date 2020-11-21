#ifndef __MARS_GPS_H__
#define __MARS_GPS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include <TinyGPS++.h>
#include <SoftwareSerial>

#define GPS_RX 5
#define GPS_TX 4
#define GPS_BAUD 9600
#define GPS_TIMEOUT 500 //milliSeconds

#ifndef USE_GPS
    #define USE_GPS true
#endif

class GPSModule : public MARSCrashableModule {
    public:
        GPSModule(RootModule &uncrashableParent, bool addSelfToParent = true) 
            : MARSCrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "G") override;
        void genericError(const char* func, const char* file, u16 failLine) override;
        bool updatePayloadData(bool forceDataUpdate) override;

    public:
        bool fetchGPSData();
        TinyGPSAltitude getAltitude();
        TinyGPSLocation getLocation();
        TinyGPSTime getTime();
        TinyGPSPlus getGPS();

    private:
        TinyGPSPlus gps;
        SoftwareSerial ss(GPS_RX, GPS_TX);

};
#endif