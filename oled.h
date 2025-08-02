#ifndef __OLED_H
#define __OLED_H

#include "main.h"

extern const uint8_t Font8x16[][16];

void OLED_Init(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Clear(void);
void OLED_PowerOff(void);

#endif
