#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include <time.h>

#include "msp432_utils.h"

// Extern variables
typedef enum {MENU, METER, BUBBLE_LEVEL, LIDAR} STATE;
extern volatile STATE CURRENT_STATE; // Current state

//*****************************************************************************
//
//! Draws the menu circles.
//
//*****************************************************************************
void drawCircle(Graphics_Context *g_sContext)
{
    Graphics_drawCircle(g_sContext, LF_MARGIN, START_HEIGHT+MARGIN*0, 5);
    Graphics_drawCircle(g_sContext, LF_MARGIN, START_HEIGHT+MARGIN*1, 5);
    Graphics_drawCircle(g_sContext, LF_MARGIN, START_HEIGHT+MARGIN*2, 5);
}

//*****************************************************************************
//
//! Clears the menu circles.
//
//*****************************************************************************
void clearCircle(Graphics_Context *g_sContext)
{
    GrContextFontSet(g_sContext, &g_sFontFixed6x8);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 28, OPAQUE_TEXT);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 32, OPAQUE_TEXT);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 56, OPAQUE_TEXT);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 60, OPAQUE_TEXT);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 84, OPAQUE_TEXT);
    Graphics_drawString(g_sContext, (int8_t *) "  ", AUTO_STRING_LENGTH, 15, 88, OPAQUE_TEXT);
    GrContextFontSet(g_sContext, &g_sFontCmss20);
}

//*****************************************************************************
//
//! Draws the splash screen
//
//*****************************************************************************
void drawSplashScreen(Graphics_Context *g_sContext)
{
    Graphics_drawImage(g_sContext, &bLong4BPP_UNCOMP, 0, 0);
    delay(2000000);
}

//*****************************************************************************
//
//! Draws the background image of the menu.
//
//*****************************************************************************
void drawImageMenu(Graphics_Context *g_sContext)
{

    Graphics_drawImage(g_sContext, &menu4BPP_UNCOMP, 0, 0);
    Graphics_drawCircle(g_sContext, 9, 117, 5);
}

//*****************************************************************************
//
//! Initializes the background image of the meter.
//
//*****************************************************************************
void drawImageMeter(Graphics_Context *g_sContext)
{

    Graphics_drawImage(g_sContext, &meter4BPP_UNCOMP, 0, 0);
    Graphics_drawCircle(g_sContext, 117, 117, 5);
}

//*****************************************************************************
//
//! Draws the background image of the bubble level.
//
//*****************************************************************************
void drawImageBubbleLevel(Graphics_Context *g_sContext)
{
    Graphics_drawImage(g_sContext, &bubbleLevel4BPP_UNCOMP, 0, 0);
    Graphics_drawCircle(g_sContext, 117, 117, 5);
}

//*****************************************************************************
//
//! Initializes the background image of the lidar.
//
//*****************************************************************************
void drawImageLidar(Graphics_Context *g_sContext)
{
    Graphics_drawImage(g_sContext, &lidar4BPP_UNCOMP, 0, 0);
    Graphics_drawCircle(g_sContext, 117, 117, 5);
}

//*****************************************************************************
//
//! Generates a delay using a for loop
//
//*****************************************************************************
void delay(int cycles)
{
    int i = 0;

    for(i=0; i<cycles; ++i);
}

//*****************************************************************************
//
//! Retrieves the current timestamp
//
//*****************************************************************************
unsigned getTimestamp()
{
    return (unsigned)time(NULL);
}
