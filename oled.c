#include "oled.h"
#include "main.h"

#define OLED_ADDR 0x3C

void OLED_WriteCmd(I2C_HandleTypeDef *hi2c, uint8_t cmd) {
  uint8_t buf[2] = {0x00, cmd};
  HAL_I2C_Master_Transmit(hi2c, OLED_ADDR, buf, 2, 100);
}

void OLED_Init(void) {
  HAL_Delay(100);
  OLED_WriteCmd(&hi2c1, 0xAE);
  OLED_WriteCmd(&hi2c1, 0x00);
  OLED_WriteCmd(&hi2c1, 0x10);
  OLED_WriteCmd(&hi2c1, 0x40);
  OLED_WriteCmd(&hi2c1, 0x81);
  OLED_WriteCmd(&hi2c1, 0xFF);
  OLED_WriteCmd(&hi2c1, 0xA1);
  OLED_WriteCmd(&hi2c1, 0xC8);
  OLED_WriteCmd(&hi2c1, 0xA6);
  OLED_WriteCmd(&hi2c1, 0xA8);
  OLED_WriteCmd(&hi2c1, 0x3F);
  OLED_WriteCmd(&hi2c1, 0xD3);
  OLED_WriteCmd(&hi2c1, 0x00);
  OLED_WriteCmd(&hi2c1, 0xD5);
  OLED_WriteCmd(&hi2c1, 0x80);
  OLED_WriteCmd(&hi2c1, 0xD9);
  OLED_WriteCmd(&hi2c1, 0xF1);
  OLED_WriteCmd(&hi2c1, 0xDA);
  OLED_WriteCmd(&hi2c1, 0x12);
  OLED_WriteCmd(&hi2c1, 0xDB);
  OLED_WriteCmd(&hi2c1, 0x40);
  OLED_WriteCmd(&hi2c1, 0x20);
  OLED_WriteCmd(&hi2c1, 0x02);
  OLED_WriteCmd(&hi2c1, 0x8D);
  OLED_WriteCmd(&hi2c1, 0x14);
  OLED_WriteCmd(&hi2c1, 0xAF);
  OLED_Clear();
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch) {
  uint8_t i, data;
  OLED_SetPos(x, y);
  for (i = 0; i < 8; i++) {
    data = Font8x16[ch - ' '][i];
    uint8_t buf[2] = {0x40, data};
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buf, 2, 100);
  }
  OLED_SetPos(x, y + 1);
  for (i = 0; i < 8; i++) {
    data = Font8x16[ch - ' '][i + 8];
    uint8_t buf[2] = {0x40, data};
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buf, 2, 100);
  }
}

void OLED_ShowString(uint8_t x, uint8_t y, char *str) {
  while (*str) {
    OLED_ShowChar(x, y, *str++);
    x += 8;
    if (x > 120) {
      x = 0;
      y += 2;
    }
  }
}

void OLED_SetPos(uint8_t x, uint8_t y) {
  OLED_WriteCmd(&hi2c1, 0xB0 + y);
  OLED_WriteCmd(&hi2c1, (x & 0xF0) >> 4 | 0x10);
  OLED_WriteCmd(&hi2c1, x & 0x0F);
}

void OLED_Clear(void) {
  uint8_t x, y;
  for (y = 0; y < 8; y++) {
    OLED_SetPos(0, y);
    for (x = 0; x < 128; x++) {
      uint8_t buf[2] = {0x40, 0x00};
      HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buf, 2, 100);
    }
  }
}

void OLED_PowerOff(void) {
  OLED_WriteCmd(&hi2c1, 0xAE);
}
