#include <stdint.h>
#include <stdbool.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "msp432_uart.h"

//*****************************************************************************
//
//! Sends a message via UART
//
//*****************************************************************************
void sendMessage(uint_fast8_t message)
{
    UART_transmitData(EUSCI_A2_BASE, message);
}

//*****************************************************************************
//
//! Sends the increment detected by the encoder
//
//*****************************************************************************
void sendMeterIncrement(float inc)
{
    // Start bit
    sendMessage(0);

    // Sign bit
    sendMessage(((inc > 0) ? 1 : 2));

    // Increment (positive)
    sendChunks(((inc < 0) ? inc*-1 : inc));

    // End bit
    sendMessage(0);
}

//*****************************************************************************
//
//! Sends the angle detected by the accelerometer
//
//*****************************************************************************
void sendBubbleLevelAngle(float angle)
{
    // Start bit
    sendMessage(50);

    // Angle
    sendChunks(angle);

    // End bit
    sendMessage(50);
}

//*****************************************************************************
//
//! Sends a message in multiple segments, each with a size of CHUNK_SIZE
//
//*****************************************************************************
void sendChunks(float value)
{
    int val = (float) value*100;
    int total = 0;

    if(val < CHUNK_SIZE)
    {
       sendMessage(val);
    }
    else
    {
       while(1)
       {
           if(total + CHUNK_SIZE <= val)
           {
               sendMessage(CHUNK_SIZE);

               total += CHUNK_SIZE;
           }
           else
           break;
       }

       if(total != val)
       {
           int left = val - total;
           total += left;

           sendMessage(left);
       }
    }
}
