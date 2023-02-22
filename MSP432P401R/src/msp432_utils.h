#ifndef MSP432_UTILS_H_
#define MSP432_UTILS_H_

#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#define MODENUM 3   // Modes number
#define MARGIN 28    // Left/top/right margin
#define CENTER 64   // Display center
#define START_HEIGHT 33
#define LF_MARGIN 21

// Splash Screen
extern const tImage bLong4BPP_UNCOMP;

// Menu
extern const tImage menu4BPP_UNCOMP;

// Meter
extern const tImage meter4BPP_UNCOMP;

// Bubble
extern const tImage bubbleLevel4BPP_UNCOMP;

// Lidar
extern const tImage lidar4BPP_UNCOMP;

// Graphic utils
void drawCircle(Graphics_Context *g_sContext);
void clearCircle(Graphics_Context *g_sContext);
void drawSplashScreen(Graphics_Context *g_sContext);
void drawImageMenu(Graphics_Context *g_sContext);
void drawImageMeter(Graphics_Context *g_sContext);
void drawImageBubbleLevel(Graphics_Context *g_sContext);
void drawImageLidar(Graphics_Context *g_sContext);

// Time utils
void delay(int cycles);
unsigned getTimestamp();

#endif
