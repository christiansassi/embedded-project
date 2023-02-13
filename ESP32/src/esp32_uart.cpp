#include <stdint.h>
#include <HardwareSerial.h>

#include "esp32_uart.h"
#include "esp32_async_server.h"
#include "esp32_lidar.h"

int status = 0;
int increment = 0;
int sign = 0;
float total = 0;

bool skip_message = false;

//*****************************************************************************
//
//! Init UART 
//! `UART_BAUDRATE` -> 115200 as default baundrate used by MSP432 for UART
//! `UART_SERIAL` -> 8 as default length (in bits) of each message
//! `UART_RXD` -> port used for receiving messages. Default set on pin 16 for UART2
//! `UART_TXD` -> port used for sending messages. Default set on pin 17 for UART2
//
//*****************************************************************************
void UARTClass::init()
{
    updateCurrentState(0);

    this->SerialPort.begin(UART_BAUDRATE, UART_SERIAL, UART_RXD, UART_TXD);

    // Handler onReceive data via UART
    this->SerialPort.onReceive([&](){
        this->handleMessageUART();
    });
}

//*****************************************************************************
//
//! Returns received message, -1 if none. It should run in loop()
//
//*****************************************************************************
int UARTClass::getMessageUART()
{
    if(this->SerialPort.available())
    return this->SerialPort.read();

    return -1;
}

//*****************************************************************************
//
//! Returns received message, -1 if none. It should run in loop()
//
//*****************************************************************************
void UARTClass::handleMessageUART()
{
    int message = UART.getMessageUART();

    // If message = -1, then there are no messages
    if(message == -1)
    return;

    switch(CURRENT_STATE)
    {
        case MENU:

            // Reset variables
            status = 0;
            increment = 0;
            sign = 0;
            total = 0;
            skip_message = false;

            update(message);

            break;
        
        case METER:

            /*
                The messages sent by the MSP432 have the following structure:
                | 0 | 1/2 | chunk | ... | chunk | 0 |
                
                0 is used as start and stop bit
                1/2 is used to specify if the number is positive or negative
                chunk is the sent number (or part of it)

                However, if 3 is received, the ESP32 will go back to the menu
            */

            if(message == 3)
            {
                update(MENU);
            }
            else if(status == 0)
            {
                // Waiting for start bit

                if(message == 0)
                status = 1;
            }
            else if(status == 1)
            {
                // Wait for sign bit
                if(message == 1 || message == 2)
                {
                    sign = message;
                    status = 2;
                }
            }
            else if(status == 2)
            {
                // Wait for chunks

                if(message == 0)
                {
                    // Stop bit

                    /* 
                        Due to hardware problems/limitations, the encoder sends the same message
                        two times. For this reason, skip_message variable tells the ESP32
                        to ignore the second message.
                    */
                    if(!skip_message)
                    {
                        float value = (float) increment / 1000;
                        value = value * ((sign == 1) ? 1 : -1);

                        total += value;
                        total = (total < 0) ? 0 : total;
                        
                        meterLastMeasure = String(total);

                        Server.setLiveMeasure(String(total), getCurrentUnit());
                    }

                    skip_message = !skip_message;

                    status = 0;
                    increment = 0;
                }
                else 
                {
                    // Reassemble chunks
                    increment += message;
                }
            }

            break;
        
        case BUBBLE_LEVEL:

            /*
                The messages sent by the MSP432 have the following structure:
                | 50 | 1/2 | chunk | ... | chunk | 50 |
                
                50 is used as start and stop bit
                1/2 is used to specify if the number is positive or negative
                chunk is the sent number (or part of it)

                However, if 100 is received, the ESP32 will go back to the menu
            */

            if(message == 100)
            {
                update(MENU);
            }
            else if(message == 50)
            {
                if(increment != 0)
                {
                    float value = (float) increment / 100;

                    bubbleLevelLastMeasure = String(value);
                    
                    Server.setLiveMeasure(String(value), getCurrentUnit());

                    increment = 0;
                }
            }
            else
            {
                increment += message;
            }

            break;
        
        case LIDAR:
            
            if(message == 1)
            {
                float distance = Lidar.getMeasureLidar();

                Lidar.sendMeasureLidar(distance);

                lidarLastMeasure = String(distance);
                Server.setLiveMeasure(String(distance), getCurrentUnit());
            }
            else if(message == 2)
            {
                update(MENU);
            }

            break;
    }
}

//*****************************************************************************
//
//! Updates current state (mode)
//
//*****************************************************************************
void UARTClass::updateCurrentState(int newState)
{
    switch(newState)
    {
        case 0:
            CURRENT_STATE = MENU;
            break;
        
         case 1:
            CURRENT_STATE = METER;
            break;
        
         case 2:
            CURRENT_STATE = BUBBLE_LEVEL;
            break;
        
         case 3:
            CURRENT_STATE = LIDAR;
            break;
    }
}

//*****************************************************************************
//
//! Retrieves current state (mode)
//
//*****************************************************************************
String UARTClass::getCurrentMode()
{
    switch(CURRENT_STATE)
    {
        case MENU:
            return "Menu";

        case METER:
            return "Meter";

        case BUBBLE_LEVEL:
            return "Bubble Level";
        
        case LIDAR:
            return "Lidar";
    }
}

//*****************************************************************************
//
//! Retrieves current unit
//
//*****************************************************************************
String UARTClass::getCurrentUnit()
{
    switch(CURRENT_STATE)
    {
        case METER:
            return "cm";

        case BUBBLE_LEVEL:
            return "°";
        
        case LIDAR:
            return "cm";
    }
}

//*****************************************************************************
//
//! Retrieves current last measure
//
//*****************************************************************************
String UARTClass::getCurrentLastMeasure()
{
    switch(CURRENT_STATE)
    {
        case METER:
            return meterLastMeasure;

        case BUBBLE_LEVEL:
            return bubbleLevelLastMeasure;
        
        case LIDAR:
            return lidarLastMeasure;
    }
}

//*****************************************************************************
//
//! Updates current state and server data
//
//*****************************************************************************
void UARTClass::update(int newState)
{
    // Update current state since all messages coming from MENU mode will change the mode
    updateCurrentState(newState);

    if(newState == MENU)
    {
        Server.setMode("-");
        Server.setLiveMeasure("-", "-");
        Server.setLastMeasure("-", "-");
    }
    // Update server
    Server.setMode(getCurrentMode());
    Server.setLiveMeasure("0", getCurrentUnit());
    Server.setLastMeasure(getCurrentLastMeasure(), getCurrentUnit());
}

//*****************************************************************************
//
//! Sends message via UART
//
//*****************************************************************************
void UARTClass::_sendMessage(int message)
{
    delay(1000);

    while(!SerialPort.availableForWrite())
        delay(1);

    SerialPort.write(message);
}

//*****************************************************************************
//
//! Sends message via UART. The message is divided in packets of 256 bits each
//
//*****************************************************************************
void UARTClass::sendMessage(float value)
{
    int val = (float) value*10;
    int total = 0;

    if(val < UART_CHUNK_SIZE)
    {
        _sendMessage(val);
    }
    else
    {
        while(1)
        {
            if(total + UART_CHUNK_SIZE <= val)
            {
                _sendMessage(UART_CHUNK_SIZE);

                total += UART_CHUNK_SIZE;
            }
            else
            break;
        }

        if(total != val)
        {
            int left = val - total;
            total += left;

            _sendMessage(left);
        }
    }
}

UARTClass UART;
