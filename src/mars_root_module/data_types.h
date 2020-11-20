//Original File: https://github.com/calw20/Arduino-Template/src/data_types/data_types.h
//Original File: Cal.W 2020-07-10 
//With use and modification under MIT

#ifndef __MARS_DATA_TYPES_GYRO_H__
#define __MARS_DATA_TYPES_GYRO_H__

#include "Arduino.h"

//Nifty little union TEMPLATE!!! allowing for a float like value but happends to be two!
//Basicity it remembers the current and previous values.
template <typename T> 
union StoredData {
    struct {
        T current;
        T previous;
    };
    T val[2];
    
    
    //Direct access to the inbuilt array
    T& operator[](int i) {
        return val[i];
    }

    /*
    bool& operator>(T x) {
        return val.current > x;
    }
    
    bool& operator<(T x) {
        return val.current < x;
    }*/


    //Move the value to previous via an assignment
    StoredData& operator=(T i){
        previous = current;
        current = i;
        return *this;
    }

    //Have the option to override/copy another StoredData
    StoredData& operator=(StoredData sd){
        current = sd.current;
        previous = sd.previous;
        return *this;
    }
};

typedef StoredData<float> fStoredData;
//typedef StoredData<int16_t> i16StoredData;

//2D Vectors
/*//! Not used so why waste space??
//? Actually I think the compiler-chain removes junk so this *could* be left in
template <typename T>
union v2d {
    struct {
        T x;
        T y;
    };
    T val[2];

    T& operator[](int i) {
        return val[i];
    }

    v2d& operator=(v2d vec){
        x = vec.x;
        y = vec.y;
        return *this;
    }
};*/

//3D Vectors
template <typename T>
union V3d {
    struct {
        T x;
        T y;
        T z;
    };
    T val[3];

    T& operator[](int i) {
        return val[i];
    }

    V3d& operator=(V3d vec){
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }
};

//typedef V3d<i16StoredData> i16SdV3d;
typedef V3d<int16_t> i16V3d;
typedef V3d<int> iV3d;
typedef V3d<float> fV3d;

struct payloadData {
    //Basic Sensor Data
    //Pressure Sensor (BMP280)
    fStoredData pressure;
    fStoredData prsAltitude;
    fStoredData temp;
    
    //Accell/Gyro (MPU6050)
    //[TODO] Make these StoredData
    i16V3d a; //Accell Data
    i16V3d g; //Gyro Data

    //GPS



    fStoredData altitude;

    //Fan Control Info - Doesn't rely change
    int fanSpeed[3];
    int transFanSpeed[3];

    //Sandwitch Info
    u8 currentFilter;
    u8 maxFilterNumber;

    //System Info
    bool hitApogee;
    bool rotateOnButton;
    bool rotateOnAltitude;
};

#endif