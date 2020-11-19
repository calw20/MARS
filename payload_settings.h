//Payload Settings
//This _should_ be all you need to edit for proper operation

#define ALT_SAMPLE_COUNT 6     //Number of samples to use when calibrating the Altitude sensor
#define STEPS_PER_ROTATION 1600 //How many steps to go from filter plate a -> b
#define APOGEE_HEIGHT 580      //When is Apogee? (m) - We may need to offset this due to drift 30000ft / 3.281 = 9144m
//[TODO] Remove this?
#define ROTATE_ON_ALTITUDE true        //Should the filter plate rotate at the altitudes below?
#define ROTATE_ON_BUTTON   true        //Should the filter plate rotate on button presses?
#define FILTER_HEIGHTS {500, 400, 300, 200, 100} //When does the sandwich need to turn (m)

#define FAN_SPEED_LIMIT 25

//Timeouts
//NOTE: If these are 0 then there will be no timeout condition
#define RADIO_TIMEOUT 5*100 //[milli]Seconds
#define ARMING_TIMEOUT 30*100 //[milli]Seconds