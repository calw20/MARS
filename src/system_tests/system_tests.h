#ifndef __MARS_SYSTEM_TESTS_H__
#define __MARS_SYSTEM_TESTS_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"
#include "../mars_root_module/mars_root_module.h"

//Grabbed from [https://github.com/Erriez/ErriezSerialTerminal] on 2020-06-2021
//Licensed under MIT
#include "../ErriezSerialTerminal/ErriezSerialTerminal.h"

#include "stepper_tests.h"
#include "gps_tests.h"
#include "fan_tests.h"

void testMode();

void unknownCommand(const char *command);
void cmdHelp();
void cmdStepper();
void cmdFans();


#endif