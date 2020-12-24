#ifndef __MARS_STEPPER_TESTS_H__
#define __MARS_STEPPER_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "../gps/gps.h"

bool gpsTest(GPSModule testGPS, unsigned long timeout);
/*
bool gpsTest(GPSModule testGPS);
bool gpsTest(GPSModule testGPS);
bool gpsTest(GPSModule testGPS);
*/

#endif