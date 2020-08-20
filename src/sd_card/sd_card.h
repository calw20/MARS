#ifndef __MARS_SD_H__
#define __MARS_SD_H__


#include "Arduino.h"
#include "../debug/debug.h"
#include "../../payload_settings.h"
#include "../mars_root_module/data_types.h"

#include <SdFat.h>
#include "../pressure_sensor/pressure_senor.h"
#include "../stepper_motor/stepper_motor.h"
#include "../accell_gyro/accell_gyro.h"

#define SD_CS_PIN 53                //SPI Chip Select Pin
#define DEL_FILE_ON_OPEN false      //Delete the file before it is opened
#define LOG_FILE_SIZE 0 //768           //Size of the split files - If 0 then it wont be split
#define LOG_FILE_FORMAT "%s_%u.%s"  //The file format string
#define LOG_FILE_NAME "flight_data" //beginning of the log file name
#define LOG_FILE_EXT  "csv"         //Extention of the log file
#define FORCE_WRITE_CYCLES 100      //How many cycles need to happen before a write is forced
#define ROLL_TO_NEXT_FILE false      //!Should every boot have its own file?

#define MAX_FLIGHT_DATA_STRING_SIZE 128

//Update to the log file name
#define SET_LOG_FILE_NUM(fileNum) \
    sprintf(logFileName, LOG_FILE_FORMAT, LOG_FILE_NAME, fileNum, LOG_FILE_EXT)

#define NEXT_FILE(...) largeFileRollover(Flight_Data_File, currentFileCount) //May not include this :/

class SDCardAdapter : public CrashableModule {
    public:
        SDCardAdapter(UnCrashable &uncrashableParent, bool addSelfToParent = true) 
            : CrashableModule(uncrashableParent, addSelfToParent) {};
        bool init() override;
        void printDebug(String = "S") override;
        void genericError(const char* func, const char* file, u16 failLine) override;

    public:
        File openFile(char* logFileName, bool delFileOnOpen = false);
        

    public:
        bool initFlightDataFile(PressureSensor &prsSensorModule, StepperMotor &sandwitchMotorModule, AccellGyro &accellGyroModule);
        void writeCSVData();
        void printCSVData(bool doUpdate = false);
        int genCSVData(bool doUpdate = true);

    public:
        char fmtedFlightData[MAX_FLIGHT_DATA_STRING_SIZE];

    private:
        SdFat sdCard;
        File FlightDataFile;
        u8 currentFileCount = 0;
        char logFileName[20];
        u8 writeCycle = 0;

        PressureSensor* prsSensor;
        StepperMotor* stpMotor;
        AccellGyro* acGyro;

};

#endif