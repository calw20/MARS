//Include the right libs
#include "payload_settings.h" //This holds all the payload settings. I.E Rotaion Heights, Apogee Height, ect
#include "src/debug/debug.h"
#include "src/mars_root_module/mars_root_module.h"
#include "src/pressure_sensor/pressure_senor.h"
#include "src/stepper_motor/stepper_motor.h"
#include "src/sd_card/sd_card.h"
#include "src/accell_gyro/accell_gyro.h"
#include "src/fan_controllers/fan_controllers.h"

//Define some noice things to have
//^You really should only have to change these...
#define SERIAL_BAUD 115200

RootModule MARS_RootModule;
PressureSensor pressureSensor(MARS_RootModule);
SDCardAdapter sdCard(MARS_RootModule);
StepperMotor sandwitch(MARS_RootModule);
AccellGyro accellGyro(MARS_RootModule);
FanController fanController(MARS_RootModule);

/*
void mainAirLoop(){
    //[TODO] Latching
    bool hitApogee = false;

     while(sandwitch.currentFilter < sandwitch.maxFilterNumber || true){ //! Forced to be always enabled
        //Get Alt/Pressure
        pressureSensor.updateAltPressure();
        if (pressureSensor.currentAltitude != pressureSensor.lastAltitude){
            DBG_FPRINT_SVLN("Current Altitude: ", pressureSensor.currentAltitude);
        }

        //Get Accell/Gyro Data
        accellGyro.getMotion();
        accellGyro.printDebug("AG");

        //Write data to SD Card
        sdCard.writeCSVData();
        
        //See if we have hit Apogee
        if (!hitApogee & pressureSensor.currentAltitude > APOGEE_HEIGHT){ //* Apogee detection enabled.
            hitApogee = true;
            DBG_FPRINT_SVLN("Just hit Apogee at: ", pressureSensor.currentAltitude);
            DBG_FPRINTLN("Engaging the sandwich!");
            
        }

        //Rotate on button press
        if (rotateOnButton && digitalRead(BUTTON_PIN) & false){ //!Disabled - Enabled in ./payload_settings.h
            DBG_FPRINTLN("Button has been pressed!");
            digitalWrite(LED_PIN, HIGH);
            NEXT_FILTER(); //Next Filter
            
            while(digitalRead(BUTTON_PIN));
            digitalWrite(LED_PIN, LOW);
            DBG_FPRINTLN("Button has been released!");
        }

        //^ I hope this works (´。＿。｀)
        //\ (╯°□°）╯︵ ┻━┻) - The drift is killing me
        //Rotate once we have passed the right Altitudes
        if(rotateOnAltitude && hitApogee){ //* Enable - Enabled in  ./payload_settings.h 
            if (currentAltitude < filterHeights[currentFilter]){
                DBG_FPRINT_SV("Reached current filter height ", currentAltitude);
                DBG_FPRINT_SVLN(", for filter number ", currentFilter);
                NEXT_FILTER(); //Next Filter
            }
        }
    }
    
}*/

void mainAirLoop(){   
    
};

void setup(){
    //Serial Setup
    #if DO_DEBUG
        Serial.begin(SERIAL_BAUD);
        DBG_FPRINTFN("Build Date: ", "%s", BUILD_DATE);
        DBG_FPRINTFN("Build Version: ", "%s", BUILD_VERSION);
    #endif

    //Build and Initialize all the modules
    DBG_FPRINTLN("Begining Initialization....");
    MARS_RootModule.init();
    sdCard.initFlightDataFile(pressureSensor, sandwitch, accellGyro);

    //[TODO] Latching {Or further down}
    
    //Blink the light ~5? times
    for(int i = 0; i <= 12 ; i++){
        digitalWrite(13, (i%2 == 1)); //Shhh whilst this *could* be non-const whats the point?
        //delay(200); //This slows things down..... *wow* :/
    }

    DBG_FPRINTLN("System is setup and configured!");

    //mainAirLoop();

    while(1){
        for(int i = 0; i<=180; i++){
            DBG_FPRINT_SVLN("Speed: ", i);
            fanController.setSpeed(i);
            delay(100);
        }
        for(int i = 180; i>=0; i--){
            DBG_FPRINT_SVLN("Speed: ", i);
            fanController.setSpeed(i);
            delay(100);
        }
    }

    /*
    while(1){
        //sdCard.writeCSVData();
        pressureSensor.updateAltPressure();
        accellGyro.getMotion();
        sdCard.printCSVData(true);
    }*/
    
}

//NOP
void loop(){}