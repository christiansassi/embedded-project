#ifndef ESP32_LIDAR_H
#define ESP32_LIDAR_H

#define LIDAR_NAME "VL53LXX-V2"

//*****************************************************************************
//
//! See https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h
//! for more informations about the different configurations of the sensor
//
//*****************************************************************************

#include <Adafruit_VL53L0X.h>

class LidarClass
{
    private:

        Adafruit_VL53L0X lox;
        VL53L0X_RangingMeasurementData_t measure;

        uint8_t address;

    public:

        void init();
        float getMeasureLidar();
        void sendMeasureLidar(float distance);
};

extern LidarClass Lidar;

#endif