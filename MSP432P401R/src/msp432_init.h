#ifndef MSP432_INIT_H_
#define MSP432_INIT_H_

#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#define portClk GPIO_PORT_P4
#define pinClk GPIO_PIN0
#define portDt GPIO_PORT_P4
#define pinDt GPIO_PIN2

void _buttonsInit();
void _adcInit();
void _adcJoystickInit();
void _adcBubbleInit();
void _graphicsInit(Graphics_Context *g_sContext);
void _encoderInit();
void _ledInit();
void _UARTInit();

void hwInit(Graphics_Context *g_sContext);

#endif
