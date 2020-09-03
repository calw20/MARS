#include "fan_controllers.h"

//Makes things look neater I guess?
bool FanController::init(){
    /*
    pinMode(STEP_PULSE_PIN, OUTPUT); //StepMC Pulse Pin
    pinMode(STEP_DIREC_PIN, OUTPUT); //StepMC Direction Pin
    pinMode(STEP_ENABL_PIN, OUTPUT); //StepMC Enable Pin
    */
    DBG_FPRINTLN("Initializing Fan Controllers...");
        ESC.attach(ESC_PIN,1000,2000);
    
    DBG_FPRINTLN("Arming Fan Controllers...");
        setSpeed(0);
    
    #if DO_DEBUG
        DBG_FPRINT("Please Wait");
            delay(400);
        DBG_FPRINT(".");
            delay(400);
        DBG_FPRINT(".");
            delay(400);
        DBG_FPRINT(".");
            delay(300);
    #else
        delay(1500); //Wait for amring
    #endif
    DBG_FPRINTLN("Armed and Operational.");
    
    DBG_FPRINT("Initialized Fan Controllers.");

    return true;
}

void FanController::genericError(const char* func, const char* file, u16 failLine){
    DBG_FPRINTLN("################## Fan Controllers Error Info ##################");
    DBG_FPRINTF("The Fan Controllers have experienced an Error of type: ","[%s]\r\nMore Details;\r\n",
        CrashTypeText[status]); //DBG_PRINTLN();
    printErrorInfo(func, file, failLine);
    printDebug("-H");
    DBG_PRINTLN();
}

void FanController::printDebug(String printValues){
    //^ Defined in debug.h? //#define CHK_LETTER(letter) printValues.indexOf(letter) > -1
    if (CHK_LETTER("-H")) printValues = "I";

    if (CHK_LETTER("H")) DBG_FPRINTLN("===================== Fan Controllers Info =====================");
    
    if (CHK_LETTER("I")){
        DBG_FPRINT_SVLN("Current Fan Speed: ", currentSpeed);
    }
}

int FanController::getSpeed(){
    //[TODO] Make Consts - The inverse of below
    return map(currentSpeed, LOWER_SERVO_MAP, UPPER_SERVO_MAP, lowerESCmap, upperESCmap);
}

void FanController::setSpeed(int newSpeed){
    //[TODO] Make Consts
    currentSpeed = map(newSpeed, lowerESCmap, upperESCmap, LOWER_SERVO_MAP, UPPER_SERVO_MAP);
    DBG_FPRINT_SVLN("New FanSpeed: ", currentSpeed);
    ESC.write(currentSpeed);
}


#define ECHO_SERIAL true
void FanController::testRunLoop(){
    bool doLoop = true;
    String serialCommand = "";
    int val = 0;
    DBG_FPRINTLN("Entering Fan Debug Loop.");
    DBG_FPRINTLN("Enter H for Help.");
    Serial.print(">");

    while (doLoop){
        while (Serial.available() > 0) {
            char inChar = Serial.read(); //Grab the next char
            
            //Only echo the recived char if its wanted. 
            if (ECHO_SERIAL)
                Serial.print(inChar);
            
            //Only add to the string if the char is not a space or comma
            if (!(inChar == '\n' || inChar == '\r' || inChar == ' ' || inChar == ','))
                serialCommand += inChar;

            //If we get a newline (enter?) or a comma then we want to proccess the command
            if (inChar == '\n' || inChar == '\r' || inChar == ','){
                DBG_FPRINT_SVLN("New Command: ", serialCommand);

                char cmdChar = serialCommand.charAt(0);
                serialCommand.remove(0,1);
                int val = serialCommand.toInt();

                switch(cmdChar){
                    case 'V':
                        setSpeed(val);
                        break;

                    case 'U':
                        upperESCmap = val;
                        break;

                    case 'L':
                        lowerESCmap = val;
                        break;
                    
                    case 'D':
                        upperESCmap = UPPER_D_ESC_MAP;
                        lowerESCmap = LOWER_D_ESC_MAP;
                        setSpeed(LOWER_D_ESC_MAP);

                    case 'R':
                        //My printf function seems to not like something in that mess
                        //So its the 
                        Serial.print("{");
                            Serial.print("\"V\":"); Serial.print(currentSpeed);
                            Serial.print(",\"VT\":"); Serial.print(getSpeed());
                            Serial.print(",\"U\":"); Serial.print(upperESCmap);
                            Serial.print(",\"L\":"); Serial.print(lowerESCmap);
                            Serial.print(",\"D\":[");
                                Serial.print(UPPER_D_ESC_MAP);
                                Serial.print(",");
                                Serial.print(LOWER_D_ESC_MAP);
                            Serial.print("]");
                        Serial.println("}");
                        break;

                    case 'E':
                        doLoop = false;
                        DBG_FPRINTLN("Exiting Fan Control Loop!");
                        DBG_FPRINTLN("You may need to restart the Arduino!!");
                        break;

                    case 'H':
                        DBG_FPRINTLN("Enter a command letter/s followed by either a ',' or enter.");
                        DBG_FPRINTLN("H - Help");
                        DBG_FPRINTLN("D - Rest to the default values.");
                        DBG_FPRINTLN("R - Print values in JSON form.");
                        DBG_FPRINTLN("V00 - Fan speed value [Requires an Integer]");
                        DBG_FPRINTLN("U00 - Set the maps upper limit [Requires an Integer]");
                        DBG_FPRINTLN("L00 - Set the maps lower limit [Requires an Integer]");
                        DBG_FPRINTLN("E - Exit the Test Loop. - This may require an Arduino to be restarted.");
                        break;

                    default:
                        DBG_FPRINT_SVLN("Unknown Command: ", cmdChar + serialCommand);
                        DBG_FPRINTLN("Enter H for help.");
                }
                val = 0;
                serialCommand = "";
                if (!(cmdChar == 'E')) Serial.print(">");
            }
        }

        while (Serial.available() == 0) {
            ESC.write(currentSpeed);
        }        
    }
}