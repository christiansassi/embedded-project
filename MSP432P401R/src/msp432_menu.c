#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include <stdio.h>

#include "msp432_menu.h"
#include "msp432_utils.h"

int CURSOR = 1; // Cursor position

int WAIT_POS = 0; // If 1 it waits until the joystick has been returned to INIT_POS
int INIT_POS = -1; // Initial position (necessary for calibration and first print)
int INIT_IMG = 0; // Used to let the background images be initialized before drawn graphics compare

int BOX[MODE_NUM]; // State of each item

// Extern variables
typedef enum {MENU, METER, BUBBLE_LEVEL, LIDAR} STATE;
extern volatile STATE CURRENT_STATE; // Current state

//*****************************************************************************
//
//! Updates the main menu based on the movement of the joystick
//
//*****************************************************************************
void updateMenu(Graphics_Context *g_sContext)
{
    int i = 0;

    for(i=0; i<MODE_NUM; ++i)
    {

        // Define the texture for each menu items
        if(BOX[i] == 0 && i == CURSOR - 1)
        {
            // Filled button
            clearCircle(g_sContext);
            drawCircle(g_sContext);
            Graphics_drawCircle(g_sContext, LF_MARGIN, START_HEIGHT+MARGIN*i, 3);
            Graphics_fillCircle(g_sContext, LF_MARGIN, START_HEIGHT+MARGIN*i, 3);

            BOX[i] = 1;
        }
        else
        {
            // Empty button
            BOX[i] = 0;
        }
    }
}

//*****************************************************************************
//
//! Handles the interrupt generated by the Joystick and adapts it to the menu
//
//*****************************************************************************
void joystickMenu(Graphics_Context *g_sContext)
{
    int pos = ADC14_getResult(ADC_MEM1) / 100;
    int cursor = CURSOR;

    if(INIT_POS == -1)
    {
        // If it's the first time that the menu is drawn
        updateMenu(g_sContext);

        // Stores the default position of the joystick
        if(INIT_IMG <= 2)
            INIT_IMG++;
        else
            INIT_POS = pos;
    }
    else
    {
        // Calculate the delta of the joystick where delta is given by INIT_POS - actual position

        int16_t delta = INIT_POS - pos;
        int16_t pos_delta = (delta >= 0) ? delta : delta * -1;
        int16_t treshold = SENSITIVITY;

        // Wait until the joystick hasn't returned in its default position
        if(WAIT_POS && pos_delta <= SENSITIVITY-10)
            WAIT_POS = 0;

        if(delta > treshold && !WAIT_POS)
        {
            // If the joystick moves down
            cursor = (++cursor == MODE_NUM+1) ? 1 : cursor;
            WAIT_POS = 1;
        }
        else if(delta < -treshold && !WAIT_POS)
        {
            // If the joystick moves up
            cursor = (--cursor == 0) ? MODE_NUM : cursor;
            WAIT_POS = 1;
        }

        if(cursor != CURSOR)
        {
            // Updates current position if it has been changed
            CURSOR = cursor;
            updateMenu(g_sContext);
        }
    }
}

//*****************************************************************************
//
//! Handles the interrupt generated by the button A and adapts it to the menu
//
//*****************************************************************************
void resetMenu(Graphics_Context *g_sContext)
{
    // Reset variables associated with the menu

    INIT_IMG = 0;

    int i;

    for(i=0; i<MODE_NUM; ++i)
        BOX[i] = 0;

    drawImageMenu(g_sContext);
    updateMenu(g_sContext);
}
