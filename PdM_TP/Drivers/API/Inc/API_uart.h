#ifndef API_UART_H
#define API_UART_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart2;

#define UART_BUFFER_SIZE 64  // Tamaño del buffer

typedef bool bool_t;

bool_t uartInit();
void uartSendString(uint8_t *pstring);
void uartSendStringSize(uint8_t *pstring, uint16_t size);
void uartReceiveStringSize(uint8_t *pstring, uint16_t size);

#endif // API_UART_H
