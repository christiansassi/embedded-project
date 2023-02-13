#ifndef MSP432_UART_H_
#define MSP432_UART_H_

#define CHUNK_SIZE 255 // Change this based on the number of bits that you send. In this case for 8 bits the maximum is 255

void sendMessage(uint_fast8_t message);

void sendMeterIncrement(float inc);
void sendBubbleLevelAngle(float angle);

void sendChunks(float value);

#endif
