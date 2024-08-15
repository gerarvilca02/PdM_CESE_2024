#ifndef API_ADC_H
#define API_ADC_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;

void ADC_Init(void);
uint16_t ADC_Read(void);

#endif // API_ADC_H
