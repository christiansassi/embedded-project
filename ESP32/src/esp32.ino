#include "esp32_async_server.h"
#include "esp32_lidar.h"
#include "esp32_uart.h"

#define BAUD 115200

void setup()
{
    Serial.begin(BAUD);

    UART.init();
    Lidar.init();

    Server.startServer();
}

void loop()
{
    UART.handleMessageUART();
}
