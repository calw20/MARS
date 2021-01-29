#ifndef __MARS_FAN_TESTS_H__
#define __MARS_FAN_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

#include "../fan_controllers/fan_controllers.h"

#ifndef TEST_FAN_TIMEOUT
    #define TEST_FAN_TIMEOUT 3000 //ms
#endif

bool fanTest(FanController testController, unsigned long timeout);


#endif