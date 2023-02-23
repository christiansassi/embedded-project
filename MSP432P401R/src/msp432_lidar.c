#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include <stdio.h>

#include "msp432_lidar.h"
#include "msp432_uart.h"
#include "msp432_utils.h"

typedef enum {MENU, METER, BUBBLE_LEVEL, LIDAR} STATE;
extern volatile STATE CURRENT_STATE;

//*****************************************************************************
//
//! Lidar initialization
//
//*****************************************************************************
void lidarInit(Graphics_Context *g_sContext)
{
    // Init Lidar graphics
    drawImageLidar(g_sContext);
    displayMeasure(g_sContext);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

    lidar(g_sContext);

    sendMessage(11);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

}

//*****************************************************************************
//
//! Displays given distance
//
//*****************************************************************************
void displayMeasure(Graphics_Context *g_sContext)
{
    // Display measure
    Graphics_drawStringCentered(g_sContext, (int8_t *) "         ",AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);

    char string[10];
    sprintf(string, "Measuring");

    Graphics_drawStringCentered(g_sContext, (int8_t *) string,AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);
}

//*****************************************************************************
//
//! Manages startup and shutdown of the measurement
//
//*****************************************************************************
void lidar(Graphics_Context *g_sContext)
{
    while(CURRENT_STATE == LIDAR)
    {
        sendMessage(10);
    }
}



