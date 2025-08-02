#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_I2C1_Init(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
