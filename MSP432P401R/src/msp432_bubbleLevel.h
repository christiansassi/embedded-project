#ifndef MSP432_BUBBLELEVEL_H_
#define MSP432_BUBBLELEVEL_H_

#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#define N_CAPTURES 200 // The minimum number of captures after which a new angle is displayed

// This two variables define the range in which the bubble can be considered as centered
#define MIN_CENTERED 54
#define MAX_CENTERED 62

// This two variables define the rectangle wehere the bubble is moving
#define X_MAX_RECTANGLE 114
#define X_MIN_RECTANGLE 14

// this two variables define the maximum and minimum values returned by the ADC
#define X_MAX 10480
#define X_MIN 6000

#define SIDE 12 // Side of the bubble
#define ANGLE_WIDTH 90.0

#define MAX_ANGLE 45
#define CALIBRATION_OFFSET 1.3

void drawBubbleRect(Graphics_Context *g_sContext);
void bubble(Graphics_Context *g_sContext);
void bubbleInit(Graphics_Context *g_sContext);

#endif
