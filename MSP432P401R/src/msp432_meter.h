#ifndef MSP432_METER_H_
#define MSP432_METER_H_

#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

void printMeasure(Graphics_Context *g_sContext, float measure);
void encoder(Graphics_Context *g_sContext);
void meter(Graphics_Context *g_sContext);

#endif
