#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include <stdio.h>
#include <math.h>

#include "msp432_bubbleLevel.h"

#include "msp432_utils.h"
#include "msp432_uart.h"

int pos = 15; // Position of the bubble
int oldPos = 0;

volatile int count = 0; // It is used to show an update of the bubble every N_CAPTURES captures

Graphics_Rectangle rectBubble; // Rectangle used to display the bubble

// Extern variables
extern uint16_t xMeasured = 0;

//*****************************************************************************
//
//! Displays the bubble
//
//*****************************************************************************
void drawBubbleRect(Graphics_Context *g_sContext)
{
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);

    if (pos > oldPos)
    {
        rectBubble.xMin = oldPos;
        rectBubble.xMax = pos;
        Graphics_fillRectangle(g_sContext, &rectBubble);

    }
    else if (pos < oldPos)
    {
        rectBubble.xMin = pos + SIDE;
        rectBubble.xMax = oldPos + SIDE;
        Graphics_fillRectangle(g_sContext, &rectBubble);
    }

    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawLineV(g_sContext, 46, 54, 74);
    Graphics_drawLineV(g_sContext, 81, 54, 74);

}

//*****************************************************************************
//
//! Show bubble
//
//*****************************************************************************
void drawRect(Graphics_Context *g_sContext)
{
    // New capture -> increment counter
    count++;

    // Saving previous pos
    oldPos = pos;

    // Calculate bubble new position
    pos = (int) ((xMeasured - X_MIN)*100/(X_MAX-X_MIN)) + X_MIN_RECTANGLE - SIDE/2;

    // Calculate new angle
    float angle = ((xMeasured - X_MIN)*ANGLE_WIDTH/(X_MAX-X_MIN) - ANGLE_WIDTH/2);

    // Keep angle positive
    angle = (angle < 0) ? angle*-1 : angle;

    // Keep angle in range [0;45]
    angle = (angle > 45) ? 45 : angle;

    if (pos > X_MAX_RECTANGLE-SIDE-1)
    {
        pos = X_MAX_RECTANGLE-SIDE-1;
    }
    else if (pos < X_MIN_RECTANGLE + 1)
    {
        pos=X_MIN_RECTANGLE+1;
    }


    if (pos > MIN_CENTERED && pos < MAX_CENTERED)
    {
        // Bubble is centered
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

    }
    else
    {
        // Bubble is not centered
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    }

    drawBubbleRect(g_sContext);

    // Calculate the position of the bubble
    rectBubble.xMin = pos;
    rectBubble.xMax = pos + SIDE;

    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(g_sContext, &rectBubble);
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_RED);

    if (count == N_CAPTURES)
    {
        // Convert angle to string
        char angleString[5];
        sprintf(angleString, "%0.2f �", angle);

        // Update angle and graphics
        Graphics_drawStringCentered(g_sContext, (int8_t *) "        ", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
        Graphics_drawStringCentered(g_sContext, (int8_t *) angleString, AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);

        sendBubbleLevelAngle(angle);

        count = 0;
    }
}

//*****************************************************************************
//
//! Init bubble level
//
//*****************************************************************************
void bubbleInit(Graphics_Context *g_sContext)
{
    drawImageBubbleLevel(g_sContext);
    rectBubble.yMin = 56;
    rectBubble.yMax = 68;

    Graphics_drawStringCentered(g_sContext, (int8_t *) "        ", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(g_sContext, (int8_t *) "0.00 �", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);

}
