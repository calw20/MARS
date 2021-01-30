#ifndef __MARS_PAYLOAD_SERIAL_H__
#define __MARS_PAYLOAD_SERIAL_H__

#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "data_types.h"
#include "mars_root_module.h"

void setupSerialCodes();
void clearSerial();
void dumpPayloadDataGrid(payloadData data, bool printHeaders = true);

#ifdef USE_NON_GRID_DUMP
    void dumpPayloadData(payloadData data, bool printHeaders = true);
#endif

#endif