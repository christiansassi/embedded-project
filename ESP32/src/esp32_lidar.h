#ifndef ESP32_LIDAR_H
#define ESP32_LIDAR_H

#include <Adafruit_VL53L0X.h>

typedef Adafruit_VL53L0X::VL53L0X_Sense_config_t VL53L0X_Sense_config_t;

#define LIDAR_NAME "VL53LXX-V2"

//*****************************************************************************
//
//! See https://github.com/adafruit/Adafruit_VL53L0X/blob/master/src/Adafruit_VL53L0X.h
//! for more informations about the different configurations of the sensor
//
//*****************************************************************************
#define LIDAR_MODE VL53L0X_Sense_config_t::VL53L0X_SENSE_HIGH_ACCURACY

class LidarClass
{
    private:

        Adafruit_VL53L0X lox;
        VL53L0X_RangingMeasurementData_t measure;
        VL53L0X_Sense_config_t mode;

        uint8_t address;

    public:

        void init();
        float getMeasureLidar();
        void sendMeasureLidar(float distance);
};

extern LidarClass Lidar;

#endif