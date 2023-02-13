#include <stdint.h>
#ifndef ESP32_UART_H
#define ESP32_UART_H

#define UART_VERSION 2
#define UART_BAUDRATE 115200
#define UART_SERIAL SERIAL_8N1
#define UART_RXD 16
#define UART_TXD 17
#define UART_CHUNK_SIZE 255

class UARTClass
{
    private:

        typedef enum {MENU, METER, BUBBLE_LEVEL, LIDAR} STATE;
        STATE CURRENT_STATE;

        String meterLastMeasure = "-";
        String bubbleLevelLastMeasure = "-";
        String lidarLastMeasure = "-";

        HardwareSerial SerialPort = HardwareSerial(UART_VERSION);

        void updateCurrentState(int newState);

        String getCurrentMode();
        String getCurrentLastMeasure();
        String getCurrentUnit();

        void update(int newState);
        
        void _sendMessage(int message);

    public:

        void init();

        int getMessageUART();
        void sendMessage(float value);

        void handleMessageUART();
};

extern UARTClass UART;

#endif
