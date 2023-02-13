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

extern int LIDAR_STATE = 0; // 0 if it's receiving data from the lidar sensor, 0 otherwise

//*****************************************************************************
//
//! Lidar initialization
//
//*****************************************************************************
void lidarInit(Graphics_Context *g_sContext)
{
    // Init Lidar graphics
    drawImageLidar(g_sContext);
    displayMeasure(g_sContext, 0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

    // Start lidar
    LIDAR_STATE = 1;
}

//*****************************************************************************
//
//! Displays given distance
//
//*****************************************************************************
void displayMeasure(Graphics_Context *g_sContext, float distance)
{
    // Display measure
    Graphics_drawStringCentered(g_sContext, (int8_t *) "         ",AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);

    char string[5];
    sprintf(string, "%0.1f cm", distance);

    Graphics_drawStringCentered(g_sContext, (int8_t *) string,AUTO_STRING_LENGTH, CENTER, START_HEIGHT+27, OPAQUE_TEXT);
}




