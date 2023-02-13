#include "msp.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_I2C.h"
#include "LcdDriver/HAL_TMP006.h"

#include "msp432_init.h"
#include "msp432_utils.h"

extern const tImage bLong4BPP_UNCOMP;

const eUSCI_UART_ConfigV1 uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    13,                                      // BRDIV = 13
    0,                                       // UCxBRF = 0
    37,                                      // UCxBRS = 37
    EUSCI_A_UART_NO_PARITY,                  // No Parity
    EUSCI_A_UART_LSB_FIRST,                  // MSB First
    EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
    EUSCI_A_UART_MODE,                       // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
    EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length

};

//*****************************************************************************
//
//! Initialization of buttons A and B
//
//*****************************************************************************
void _buttonsInit()
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);

    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT5);
}

//*****************************************************************************
//
//! Initialization of Analog-to-Digital Converter (ADC) for the joystick
//
//*****************************************************************************

void _adcJoystickInit()
{
    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat) with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    ADC14_configureConversionMemory(ADC_MEM0,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM1,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);
}

void _adcInit()
{
    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
    //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1,GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configures Pin 6.0 and 4.4 as ADC input */
    //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8,0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
     * with 3.3v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);
    //ADC14_configureConversionMemory(ADC_MEM1,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);
    //ADC14_configureConversionMemory(ADC_MEM2,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);
    //ADC14_configureConversionMemory(ADC_MEM3,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM1,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 2 (end of sequence) is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence convert. */
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

//*****************************************************************************
//
//! Initialization of the Graphic and Splash Screen
//
//*****************************************************************************
void _graphicsInit(Graphics_Context *g_sContext)
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(g_sContext, &g_sCrystalfontz128x128,&g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(g_sContext, &g_sFontCmss20);
    Graphics_clearDisplay(g_sContext);

    /* Splash Screen */
    drawSplashScreen(g_sContext);

    /* Draw Menu */
    drawImageMenu(g_sContext);

    Interrupt_enableMaster();
}

//*****************************************************************************
//
//! Initialization of the Encoder for the meter measurements
//
//*****************************************************************************
void _encoderInit()
{
    GPIO_setAsInputPinWithPullUpResistor(portClk, pinClk);   //P4.0
    GPIO_setAsInputPinWithPullUpResistor(portDt, pinDt);     //P6.1
}

//*****************************************************************************
//
//! Initialization of the RGB LED
//
//*****************************************************************************
void _ledInit()
{
    // Green LED (P2.4)
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN4);        // Set P2.4 as output
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);    // Set output low (green LED off)

    // Red LED (P2.6)
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);        // Set P2.6 as output
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);    // Set output low (red LED off)
}

//*****************************************************************************
//
//! Initialization of UART communication
//
//*****************************************************************************
void _UARTInit()
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    UART_enableModule(EUSCI_A2_BASE);
}

//*****************************************************************************
//
//! Hardware initialization
//
//*****************************************************************************
void hwInit(Graphics_Context *g_sContext)
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    /* Initializes all Components */
    _buttonsInit();
    _adcInit();
    _encoderInit();
    _ledInit();
    _graphicsInit(g_sContext);
    _UARTInit();
}



