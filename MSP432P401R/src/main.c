#include <msp432_init.h>
#include "msp.h"

#include "msp432_bubbleLevel.h"
#include "msp432_init.h"
#include "msp432_lidar.h"
#include "msp432_menu.h"
#include "msp432_meter.h"
#include "msp432_uart.h"
#include "msp432_utils.h"

Graphics_Context g_sContext = {};

// Buttons utils and variables
const unsigned MIN_DELAY = 1; // Minimum wait time before a button can be clicked again.
unsigned LAST_CLICK_B = 0; // Last click of button B (timestamp)
unsigned LAST_CLICK_A = 0;

/*
 * The button interrupt has been divided into multiple sections
 * to address the issue of false interrupt generated by hardware.
 * This approach provides more control over the problem and allows
 * a better management.
 */
void PORT3_IRQHandler(); // This is the function associated to the interrupt
int checkInterruptB(); // This is the function that checks the presence of an interrupt
int canClickB(); // This is the function that check the delay between more interrupt
void buttonB(); // This is the function that executes the codes associated to this button

void PORT5_IRQHandler();
int checkInterruptA();
int canClickA();
void buttonA();

// Extern variables

// Menu
typedef enum {MENU, METER, BUBBLE_LEVEL, LIDAR} STATE;
extern STATE CURRENT_STATE = MENU;

extern int CURSOR;

// Bubble Level
extern uint16_t xMeasured;

void main(void)
{
    // Hardware initialization
    hwInit(&g_sContext);

    // Splash screen + menu
    updateMenu(&g_sContext);

    // Sleep mode
    while(1)
        PCM_gotoLPM0();
}

//*****************************************************************************
//
//! Handler associated with the joystick
//
//*****************************************************************************
void ADC14_IRQHandler()
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT1)
    {
        switch(CURRENT_STATE)
        {
            case MENU:

                joystickMenu(&g_sContext);
                break;

            case BUBBLE_LEVEL:

                xMeasured = ADC14_getResult(ADC_MEM0);
                drawRect(&g_sContext);

                if(checkInterruptB())
                    buttonB();

                break;
        }
    }
}

//*****************************************************************************
//
//! Handler associated with button A
//
//*****************************************************************************
void PORT5_IRQHandler()
{
    if(checkInterruptA())
        buttonA();
}

//*****************************************************************************
//
//! Check if the button A has been clicked
//
//*****************************************************************************
int checkInterruptA()
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    return (status & GPIO_PIN1);
}

//*****************************************************************************
//
//! Check if the last click was done at least MIN_DELAY second(s) ago
//
//*****************************************************************************
int canClickA()
{
    unsigned now = getTimestamp();

    unsigned delta = now - LAST_CLICK_A;

    if(delta >= MIN_DELAY)
    LAST_CLICK_A = now;

    return (delta >= MIN_DELAY);
}

//*****************************************************************************
//
//! Function associated with button A
//
//*****************************************************************************
void buttonA()
{
    if(!canClickA())
        return;

    switch(CURRENT_STATE)
    {
        case MENU:

            CURRENT_STATE = (STATE) CURSOR;
            Graphics_clearDisplay(&g_sContext);

            sendMessage(CURRENT_STATE);

            if(CURSOR == METER)
            {
                encoderInit(&g_sContext);
                meter(&g_sContext);
                sendMessage(3);
                CURRENT_STATE = MENU;
                drawImageMenu(&g_sContext);
                resetMenu(&g_sContext);
            }
            else if(CURSOR == BUBBLE_LEVEL)
            {
                bubbleInit(&g_sContext);
            }
            else
            {
                sendMessage(1);
                lidarInit(&g_sContext);
                sendMessage(2);
                CURRENT_STATE = MENU;
                drawImageMenu(&g_sContext);
                resetMenu(&g_sContext);
            }

            break;
/*
        case METER:

            meter(&g_sContext);
            break;
*/
    }
}

//*****************************************************************************
//
//! Handler associated with button A
//
//*****************************************************************************
void PORT3_IRQHandler()
{
    if(checkInterruptB())
    buttonB();
}

//*****************************************************************************
//
//! Check if the button B has been clicked
//
//*****************************************************************************
int checkInterruptB()
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    return (status & GPIO_PIN5);
}

//*****************************************************************************
//
//! Check if the last click was done at least MIN_DELAY second(s) ago
//
//*****************************************************************************
int canClickB()
{
    unsigned now = getTimestamp();

    unsigned delta = now - LAST_CLICK_B;

    if(delta >= MIN_DELAY)
    LAST_CLICK_B = now;

    return (delta >= MIN_DELAY);
}

//*****************************************************************************
//
//! Function associated with button B
//
//*****************************************************************************
void buttonB()
{
    if(!canClickB())
        return;

    if(CURRENT_STATE == MENU)
        return;

    switch(CURRENT_STATE)
    {
        case METER:

            sendMessage(3);

            break;

        case BUBBLE_LEVEL:

            sendBubbleLevelAngle(100);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

            break;

        case LIDAR:

            //LIDAR_STATE = 1;
            //sendMessage(2);
            //GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);

            break;
    }

    CURRENT_STATE = MENU;
    Graphics_clearDisplay(&g_sContext);
    resetMenu(&g_sContext);
}

//*****************************************************************************
//
//! Handler associated with the reception of UART messages
//
//*****************************************************************************
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        Interrupt_disableSleepOnIsrExit();
        
        int message = UART_receiveData(EUSCI_A2_BASE);

       // Incomplete due to UART problems
    }

}
