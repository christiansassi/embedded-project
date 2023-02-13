#include "esp32_lidar.h"
#include "esp32_uart.h"

//*****************************************************************************
//
//! Initializates the lidar sensor.
//
//*****************************************************************************
void LidarClass::init()
{
    this->mode = LIDAR_MODE;

    if(!this->lox.configSensor(mode))
    {
        Serial.println("Failed to configure ");
        Serial.print(LIDAR_NAME);
        Serial.print(" sensor.");

        while(1)
        delay(100);
    }
    
    if(!this->lox.begin())
    {
        Serial.println("Failed to init ");
        Serial.print(LIDAR_NAME);
        Serial.print(" sensor.");

        while(1)
        delay(100);
    }
}

//*****************************************************************************
//
//! Retrieves the detected measure from the sensor.
//! It returns -1 if out of range.
//
//*****************************************************************************
float LidarClass::getMeasureLidar()
{
    this->lox.rangingTest(&(this->measure), false);

    return (this->measure.RangeStatus != 4) ? (float) this->measure.RangeMilliMeter / 10 : -1;
}

//*****************************************************************************
//
//! Sends distance to the MSP432
//
//*****************************************************************************
void LidarClass::sendMeasureLidar(float distance)
{
    // Start bit
    UART.sendMessage(0);

    // Increment (positive)
    UART.sendMessage(distance);

    // End bit
    UART.sendMessage(0);
}

LidarClass Lidar;
