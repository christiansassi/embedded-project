#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include <stdio.h>

#include "msp432_meter.h"
#include "msp432_utils.h"
#include "msp432_init.h"
#include "msp432_uart.h"

const uint16_t wheelRadius  = 15; // Wheel radius (in millimeter)
const float pi = 3.14159265358979323846264338327950288; // Pi value

float measure = 0.0; // Distance in Millimeter
float inc = 0.0; // Measure increment at each step

int START = 0; // Control variable
extern int METER_STATE = 1; // It defines if the meter is in start / stop state


//*****************************************************************************
//
//! Prints the measurement data on the display (cm only)
//
//*****************************************************************************

void printMeasure(Graphics_Context *g_sContext)
{
    Graphics_drawStringCentered(g_sContext, (int8_t *) "         ",AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);

    char string[5]; // String to print on the display
    sprintf(string, "%0.2f cm", measure/10);

    Graphics_drawStringCentered(g_sContext, (int8_t *) string,AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);
}

//*****************************************************************************
//
//! Encoder initialization
//
//*****************************************************************************
void encoderInit(Graphics_Context *g_sContext)
{
    measure = 0;
    START = 1;
    METER_STATE = 1;

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

    drawImageMeterInit(g_sContext);
    printMeasure(g_sContext);

}

//*****************************************************************************
//
//! Handles encoder signals
//
//*****************************************************************************
void encoderHandler(Graphics_Context *g_sContext)
{
    if(!START)
        return;

    // Define increment
    inc = (2*pi*wheelRadius)/20;

    drawImageMeterMeasure(g_sContext);
    printMeasure(g_sContext);

    int prevClk, prevDt;

    prevClk = GPIO_getInputPinValue(portClk, pinClk);
    prevDt = GPIO_getInputPinValue(portDt, pinDt);

    int i=0;

    while(1){
        // Clock and Data signal from the encoder
        int currClk = GPIO_getInputPinValue(portClk, pinClk);
        int currDt = GPIO_getInputPinValue(portDt, pinDt);

        // If the encoder has been rotated
        if (currClk != prevClk)
        {
            if (currDt == currClk)
            {
                // Clockwise means increment
                (i == 0 ? (measure += inc, i++) : (i = 0));
                sendMeterIncrement(inc);
            } else
            {
                // Counterclockwise means decrement
                (i == 0 ? (measure -= inc, i++) : (i = 0));
                sendMeterIncrement(-inc);
            }

            prevClk = currClk;
            prevDt = currDt;

            // Keep captured measure greater or equal than 0
            measure = (measure < 0) ? 0 : measure;

            printMeasure(g_sContext);
        }

        if(checkInterruptA())
        {
            printf("Interrupt\n");

            if(canClickA())
            {
                printf("Click\n");
                break;
            }
        }
    }
}

//*****************************************************************************
//
//! Manages startup and shutdown of the measurement
//
//*****************************************************************************
void meter(Graphics_Context *g_sContext)
{

    METER_STATE = !METER_STATE;

    if(METER_STATE)
    {
        //Turn on Green LED
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
        encoderHandler(g_sContext);
        // Turn off Green LED

        encoderInit(g_sContext);

        METER_STATE = !METER_STATE;
    }
}

