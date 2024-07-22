#include "API_debounce.h"
#include "API_delay.h"
#include "main.h" // Incluir el encabezado donde están definidos los pines

// Definición de estados de la MEF
typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RAISING
} debounceState_t;

// Variables globales privadas
static debounceState_t currentState;
static bool_t buttonPressedFlag;
static delay_t debounceDelay;

void debounceFSM_init()
{
    currentState = BUTTON_UP;
    delayInit(&debounceDelay, 40); // 40 ms anti-rebote
    buttonPressedFlag = false;
}

void debounceFSM_update()
{
    switch (currentState)
    {
    case BUTTON_UP:
        if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
        {
            currentState = BUTTON_FALLING;
            delayWrite(&debounceDelay, 40);
        }
        break;
    case BUTTON_FALLING:
        if (delayRead(&debounceDelay))
        {
            if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
            {
                currentState = BUTTON_DOWN;
                buttonPressedFlag = true;
                buttonPressed(); // Encender el LED
            }
            else
            {
                currentState = BUTTON_UP;
            }
        }
        break;
    case BUTTON_DOWN:
        if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
        {
            currentState = BUTTON_RAISING;
            delayWrite(&debounceDelay, 40);
        }
        break;
    case BUTTON_RAISING:
        if (delayRead(&debounceDelay))
        {
            if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
            {
                currentState = BUTTON_UP;
                buttonReleased(); // Apagar el LED
            }
            else
            {
                currentState = BUTTON_DOWN;
            }
        }
        break;
    }
}

bool_t readKey()
{
    if (buttonPressedFlag)
    {
        buttonPressedFlag = false;
        return true;
    }
    return false;
}

void buttonPressed()
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); // Encender el LED
}

void buttonReleased()
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Apagar el LED
}
