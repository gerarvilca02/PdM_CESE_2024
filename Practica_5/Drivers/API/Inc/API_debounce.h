#ifndef API_DEBOUNCE_H
#define API_DEBOUNCE_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

void debounceFSM_init();
void debounceFSM_update();
bool_t readKey();
void buttonPressed();
void buttonReleased();

#endif // API_DEBOUNCE_H
