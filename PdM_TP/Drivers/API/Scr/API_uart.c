#include "API_uart.h"
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

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

    // Limpieza del terminal
    uint8_t clearCommand[] = "\033[2J\033[H";
    HAL_UART_Transmit(&huart2, clearCommand, sizeof(clearCommand) - 1, HAL_MAX_DELAY);

    // Mostrar mensajes de bienvenida
    uint8_t welcomeMessage[] = "BIENVENIDO\r\n";
    HAL_UART_Transmit(&huart2, welcomeMessage, sizeof(welcomeMessage) - 1, HAL_MAX_DELAY);
    HAL_Delay(1500);

    HAL_UART_Transmit(&huart2, clearCommand, sizeof(clearCommand) - 1, HAL_MAX_DELAY);

    uint8_t initMessage[] = "SISTEMA DE RIEGO AUTOMÁTICO\r\n";
    HAL_UART_Transmit(&huart2, initMessage, sizeof(initMessage) - 1, HAL_MAX_DELAY);
    HAL_Delay(1500);

    HAL_UART_Transmit(&huart2, clearCommand, sizeof(clearCommand) - 1, HAL_MAX_DELAY);

    // Mostrar Nombre y curso
    uint8_t presentacionMessage[] = "TP PdM - Gerardo Vilcamiza\r\n";
    HAL_UART_Transmit(&huart2, presentacionMessage, sizeof(presentacionMessage) - 1, HAL_MAX_DELAY);
    HAL_Delay(1500);

    // Limpiar pantalla
    HAL_UART_Transmit(&huart2, clearCommand, sizeof(clearCommand) - 1, HAL_MAX_DELAY);

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
