#include "serial_handling.h"

void setupSerialCodes(){
    Serial.print("\e[2J");   //Clear the screen
    Serial.print("\eSP F");  // tell to use 7-bit control codes
    Serial.print("\e[?25l"); // hide cursor
    Serial.print("\e[?12l"); // disable cursor highlighting
}

void clearSerial(){
    Serial.print("\e[2J");  //Clear the screen
    Serial.print("\e[1;1"); //Move curser to (1,1) ~ (0,0)
}

void dumpPayloadDataGrid(payloadData data){
    Serial.print(F("\e[1;1HMARS Payload Data"));
    Serial.print(F("\e[2;1HBuild Date: ")); Serial.print(F(BUILD_DATE));
    Serial.print(F("\e[3;1HBuild Version: ")); Serial.print(F(BUILD_VERSION));
    
    Serial.print(F("\e[5;1HGPS Lat: \e[5;11H")); Serial.print(data.position[0], 6);
    Serial.print(F("\e[6;1HGPS Long: \e[6;11H")); Serial.print(data.position[1], 6);
    Serial.print(F("\e[7;1HGPS Altitude: \e[7;15H")); Serial.print(data.altitude[0]);
    Serial.print(F("\e[9;1HRun Time (s): \e[9;15H")); Serial.print((double)(millis()/1000.00));
    Serial.print(F("\e[8;1HUTC Timestamp: \e[8;16H")); Serial.print(data.time);
    Serial.print(F("\e[10;1HGPS Ground Level Offset: \e[10;27H")); Serial.print(data.altGndLvlOffset);
    
    Serial.print(F("\e[12;1HPressure: \e[12;11H")); Serial.print(data.pressure[0]);
    Serial.print(F("\e[13;1HFan Speed: \e[13;12H")); Serial.print(data.fanSpeed[0]);
    Serial.print(F("\e[14;1HTemperature: \e[14;14H")); Serial.print(data.temp[0]);
    
    Serial.print(F("\e[16;1HGyro X: \e[16;10H")); Serial.print(data.g[0]);
    Serial.print(F("\e[17;1HGyro Y: \e[17;10H")); Serial.print(data.g[1]);
    Serial.print(F("\e[18;1HGyro Z: \e[18;10H")); Serial.print(data.g[2]);
    Serial.print(F("\e[19;1HAccell X: \e[19;11H")); Serial.print(data.a[0]);
    Serial.print(F("\e[20;1HAccell Y: \e[20;11H")); Serial.print(data.a[1]);
    Serial.print(F("\e[21;1HAccell Z: \e[21;11H")); Serial.print(data.a[2]);

    Serial.print(F("\e[23;1HCurrent Filter: \e[23;17H")); Serial.print(data.currentFilter);
    Serial.print(F("\e[24;1HNum of Filters: \e[24;17H")); Serial.print(data.maxFilterNumber);
    
    Serial.print(F("\e[26;1HHit Apogee? \e[26;13H")); Serial.print(data.hitApogee ? F("Yes") : F("No"));
    Serial.print(F("\e[27;1HRotate on Button?    \e[27;22H")); Serial.print(data.rotateOnButton ? F("Yes") : F("No"));
    Serial.print(F("\e[28;1HRotate on Altitude?  \e[28;22H")); Serial.print(data.rotateOnAltitude ? F("Yes") : F("No"));
}

void dumpPayloadData(payloadData data){
    Serial.print(F("MARS Payload Data"));
    Serial.print(F("Build Date: ")); Serial.println(BUILD_DATE);
    Serial.print(F("Build Version: ")); Serial.println(BUILD_VERSION);

    Serial.println();

    Serial.print(F("GPS Lat:  ")); Serial.println(data.position[0], 6);
    Serial.print(F("GPS Long: ")); Serial.println(data.position[1], 6);
    Serial.print(F("GPS Altitude:  ")); Serial.println(data.altitude[0]);
    Serial.print(F("Run Time (s):  ")); Serial.println((double)(millis()/1000.00));
    Serial.print(F("UTC Timestamp: ")); Serial.println(data.time);
    Serial.print(F("GPS Ground Level Offset: ")); Serial.println(data.altGndLvlOffset);

    Serial.println();
    
    Serial.print(F("Pressure:  ")); Serial.println(data.pressure[0]);
    Serial.print(F("Fan Speed: ")); Serial.println(data.fanSpeed[0]);
    Serial.print(F("Temperature: ")); Serial.println(data.temp[0]);

    Serial.println();

    Serial.print(F("Gyro X: ")); Serial.println(data.g[0]);
    Serial.print(F("Gyro Y: ")); Serial.println(data.g[1]);
    Serial.print(F("Gyro Z: ")); Serial.println(data.g[2]);
    Serial.print(F("Accell X: ")); Serial.println(data.a[0]);
    Serial.print(F("Accell Y: ")); Serial.println(data.a[1]);
    Serial.print(F("Accell Z: ")); Serial.println(data.a[2]);

    Serial.println();

    Serial.print(F("Current Filter: ")); Serial.println(data.currentFilter);
    Serial.print(F("Num of Filters: ")); Serial.println(data.maxFilterNumber);
    
    Serial.println();

    Serial.print(F("Hit Apogee? ")); Serial.println(data.hitApogee ? F("Yes") : F("No"));
    Serial.print(F("Rotate on Button?    ")); Serial.println(data.rotateOnButton ? F("Yes") : F("No"));
    Serial.print(F("Rotate on Altitude?  ")); Serial.println(data.rotateOnAltitude ? F("Yes") : F("No"));
}
