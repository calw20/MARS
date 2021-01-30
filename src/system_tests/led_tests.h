#ifndef __MARS_LED_TESTS_H__
#define __MARS_LED_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "../ledDriver/ledDriver.h"

#ifndef TEST_LED_DELAY
    #define TEST_LED_DELAY 750 //ms
#endif

bool ledTest(RootModule rootModule, unsigned long msDelay = TEST_LED_DELAY);


#endif