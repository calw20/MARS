//Include the right libs
#include "payload_settings.h" //This holds all the payload settings. I.E Rotaion Heights, Apogee Height, ect
#include "src/debug/debug.h"
#include "src/mars_root_module/data_types.h"
#include "src/mars_root_module/mars_root_module.h"
#include "src/pressure_sensor/pressure_senor.h"
#include "src/stepper_motor/stepper_motor.h"
#include "src/sd_card/sd_card.h"
#include "src/accell_gyro/accell_gyro.h"
#include "src/fan_controllers/fan_controllers.h"
#include "src/gps/gps.h"

#include "src/wireless_module/wireless_module.h"
#include "src/wireless_radio/wireless_datatypes.h"

#include "src/mars_root_module/payload_data_formatter.h"
#include "src/system_tests/system_tests.h"

#include <SdFat.h>

//^You really should only have to change these...
#define SERIAL_BAUD 115200
#define BUTTON_PIN A0

RootModule MARS_RootModule;
SystemTestHandler systemTests;
PressureSensor pressureSensor(MARS_RootModule);
SDCardAdapter sdCard(MARS_RootModule);
StepperMotor sandwitch(MARS_RootModule);
AccellGyro accellGyro(MARS_RootModule);
FanController fanController(MARS_RootModule);
GPSModule gpsRadio(MARS_RootModule);
WirelessModule radioModule(MARS_RootModule, &systemTests, &sandwitch);

//Define a constant array - this defined in ./payload_settings.h
const float apogeeHeight = APOGEE_HEIGHT;

//Simple Redirection
TriColourLED *cLED1;
TriColourLED *cLED2;
payloadData &pData = MARS_RootModule.data;

void mainAirLoop(){
    cLED1->setColour(LEDColours::CYAN);
    while(sandwitch.isAbleToRotate()){
        MARS_RootModule.updatePayloadData(true); //Update paloadData struct
        sdCard.writeCSVData();
        //sdCard.printCSVData();
        
        //[TODO] Do wireless Logic on the pad ie alt change sml
        //if (abs(pData.altitude[0] - pData.altitude[1]) < 5) WirlessUpdate()

        //See if we have hit Apogee
        //Check if we have passed a pre-determined apogee height or if the altitude has decreased
        //No _Automatic_ Flight Logic is performed if apogee has not been passed
        if (!pData.hitApogee && (pData.altitude[0] > apogeeHeight | pData.altitude[0] < pData.altitude[1])){
            pData.hitApogee = true;
            DBG_FPRINT_SVLN("Just hit Apogee at: ", pData.altitude[0]);
            DBG_FPRINTLN("Engaging the sandwich!");
            cLED1->setColour(LEDColours::GREEN);
        }

        //Rotate on button press
        if (pData.rotateOnButton && digitalRead(BUTTON_PIN)){
            DBG_FPRINTLN("Button has been pressed!");
            cLED2->setColour(LEDColours::MAGENTA);
            sandwitch.nextFilter();

            while(digitalRead(BUTTON_PIN));
            cLED2->setColour(LEDColours::BLACK);
            DBG_FPRINTLN("Button has been released!");
        }

        //Rotate once we have passed the right Altitudes
        if(pData.rotateOnAltitude && pData.hitApogee && sandwitch.shouldRotate()){ //* Enable - Enabled in  ./payload_settings.h 
            DBG_FPRINT_SV("Reached current filter height ", pData.altitude[0]);
            DBG_FPRINT_SVLN(", for filter number ", pData.currentFilter);
            sandwitch.nextFilter();
        }

        //^ I hope this works (´。＿。｀)
        //If the sandwitch is engaged run the fans
        if(pData.currentFilter > 0 && pData.currentFilter < pData.maxFilterNumber && fanController.getSpeed() != FAN_SPEED_LIMIT)
            fanController.setSpeed(FAN_SPEED_LIMIT);
        else
            fanController.setSpeed(0);

        fanController.writeSpeed();
    }
    cLED1->setColour(LEDColours::WHITE);
    fanController.setSpeed(0);
    DBG_FPRINTLN("All samples collected!");
    DBG_FPRINTLN("Please reset the Mega.");
    MARS_RootModule.updatePayloadData(true);
    sdCard.writeCSVData();
    sdCard.closeFile();

    //[TODO] Wireless Data
    //while(true) {}
}

/*
void wirelessTest(){
    DBG_FPRINTLN("Wireless Test Mode!!");
    setupSerialCodes();
    while(1){
        radioModule.updateNetwork();
        pData.time = millis();
        pData.pressure = rand();
        pData.prsAltitude = rand();
        pData.temp = pData.temp[0] + 1;
        pData.a.z = rand();
        bool val = radioModule.sendResponse(WirelessResponses::SystemState, &pData);
        //DBG_PRINTLN(pData.time);
        //DBG_PRINTFN("Transmission State: %s", val ? "Success" : "Failed");
        
        if (val){
            dumpPayloadData(pData);
            hexDump("pData", &pData, sizeof(payloadData));
        }

        delay(3000);
    }
}*/

void setup(){
    //Serial Setup
    #if DO_DEBUG
        Serial.begin(SERIAL_BAUD);
        //while (!Serial) {} //Needed sometimes, have to test if this makes a difference
        DBG_FPRINTFN("Build Date: ", "%s", BUILD_DATE);
        DBG_FPRINTFN("Build Version: ", "%s", BUILD_VERSION);
    #endif

    systemTests.init(&MARS_RootModule, &pressureSensor, &sdCard, &sandwitch,
        &accellGyro, &fanController, &gpsRadio, &radioModule);
    systemTests.testMode();

    //radioModule.init();
    //wirelessTest();

    //Build and Initialize all the modules
    DBG_FPRINTLN("Begining Initialization....");
    MARS_RootModule.init();
    
    //[TODO] Move this to SDCard Module?
    //Write all the consts to a file
    char constFileName[14] = "constFile.csv";
    File constFile = sdCard.openFile(constFileName);
    constFile.println("UTC Date,Altitude Offset,Sea Level Pressure");
    char __constFileBuff__[128];
    sprintf(__constFileBuff__, "%lu,%lu,%f", 
        (unsigned long)gpsRadio.getDate(), (unsigned long)pData.altGndLvlOffset, pressureSensor.getSeaLevelPressure()
    );
    constFile.println(__constFileBuff__);
    constFile.flush();
    constFile.close();

    sdCard.initFlightDataFile();
    cLED1 = MARS_RootModule.cLED1;
    cLED2 = MARS_RootModule.cLED2;

    //Blink the light ~5? times
    for(int i = 0; i <= 12 ; i++){
        cLED1->setColour(((i%2 == 1) ? LEDColours::GREEN : LEDColours::BLACK));
        if (DO_INIT_DELAY) delay(200); //This slows things down..... *wow* :/
    }

    DBG_FPRINTLN("System is setup and configured!");

    //Hang until armed
    //[TODO] Enable this ¯\_(ツ)_/¯
    DBG_FPRINTLN("System not armed, wating for signal!");
    unsigned long beginArmWait = millis();
    unsigned long cLoop = beginArmWait;

    while(!MARS_RootModule.systemArmed){
        //Flash the light
        if (millis()-cLoop > 500) cLoop = millis();
        cLED2->setColour((millis()-cLoop > 500) ? LEDColours::CYAN : LEDColours::BLACK);

        //Check if there has been a button press
        if (digitalRead(BUTTON_PIN)){
            DBG_PRINTLN("Button Press Detected!");
            unsigned long buttonTime = millis();
            while(digitalRead(BUTTON_PIN)){
                if (millis() - buttonTime < 1000)
                    cLED2->setColour(LEDColours::YELLOW);
                else if (millis() - buttonTime < 1500)
                    cLED2->setColour(LEDColours::GREEN);
                else if (millis() - buttonTime < 2000)
                    cLED2->setColour(LEDColours::BLUE);
                else if (millis() - buttonTime < 2500)
                    cLED2->setColour(LEDColours::RED);
                /*else if (millis() - buttonTime < 3000)
                    cLED2->setColour(LEDColours::CYAN);*/
                else
                    cLED2->setColour(LEDColours::WHITE);
            } //Wait for button release

            switch (cLED2->getColour()){
            case LEDColours::YELLOW:
                DBG_FPRINTLN("Yellow: Armming from button press");
                MARS_RootModule.systemArmed = true;   
                break;
            case LEDColours::GREEN:
            case LEDColours::BLUE:
            case LEDColours::RED:
                DBG_FPRINTLN("Red: Entering test mode. You will have to reset the system to exit.");
                cLED2->setColour(LEDColours::BLACK);
                systemTests.testMode();
                break;
            default:
                break;
            }
        }

        //Wireless Command Handling
        radioModule.waitForHandledCommand();

        //Arming Timeout
        if (ARMING_TIMEOUT > 0 && millis()-beginArmWait > ARMING_TIMEOUT){
            DBG_PRINTLN("Arming order timed out. System will arm based on this.");
            MARS_RootModule.systemArmed = true;
        }
    }

    DBG_FPRINTLN("System armed. Begining Flight Loop!");
    
    mainAirLoop();
}

//NOP
void loop(){}