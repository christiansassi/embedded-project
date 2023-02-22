#ifndef MSP432_MENU_H_
#define MSP432_MENU_H_

#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#define SENSITIVITY 40 // Joystick sensitivity

void updateMenu(Graphics_Context *g_sContext);
void joystickMenu(Graphics_Context *g_sContext);
void buttonAMenu(Graphics_Context *g_sContext);
void resetMenu(Graphics_Context *g_sContext);

#endif
