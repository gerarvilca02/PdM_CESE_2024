#include "API_uart.h"
#include "main.h"

extern UART_HandleTypeDef huart2; // Declaración externa de huart2

static uint8_t txBuffer[UART_BUFFER_SIZE];
static uint8_t rxBuffer[UART_BUFFER_SIZE];

bool_t uartInit()
{
    // Inicialización de la UART
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        return false;
    }

    uint8_t initMessage[] = "UART Inicializado con BaudRate: 9600, WordLength: 8B, StopBits: 1, Parity: None, Mode: TX_RX\r\n";
    HAL_UART_Transmit(&huart2, initMessage, sizeof(initMessage) - 1, HAL_MAX_DELAY);

    return true;
}

void uartSendString(uint8_t *pstring)
{
    if (pstring != NULL)
    {
        size_t len = strlen((const char*)pstring);
        if (len < UART_BUFFER_SIZE)
        {
            memcpy(txBuffer, pstring, len);
            HAL_UART_Transmit(&huart2, txBuffer, len, HAL_MAX_DELAY);
        }
    }
}

void uartSendStringSize(uint8_t *pstring, uint16_t size)
{
    if (pstring != NULL && size > 0 && size <= UART_BUFFER_SIZE)
    {
        memcpy(txBuffer, pstring, size);
        HAL_UART_Transmit(&huart2, txBuffer, size, HAL_MAX_DELAY);
    }
}

void uartReceiveStringSize(uint8_t *pstring, uint16_t size)
{
    if (pstring != NULL && size > 0 && size <= UART_BUFFER_SIZE)
    {
        HAL_UART_Receive(&huart2, rxBuffer, size, HAL_MAX_DELAY);
        memcpy(pstring, rxBuffer, size);
    }
}
