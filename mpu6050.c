#include "mpu6050.h"
#include "main.h"
#include <string.h>

#define MPU6050_ADDR 0xD0
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define INT_ENABLE_REG 0x38
#define ACCEL_XOUT_H_REG 0x3B
#define GYRO_XOUT_H_REG 0x43

float accel_offset_x = 0, accel_offset_z = 0;
float angle = 0.0f;

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c) {
  uint8_t res;
  
  HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1, (uint8_t*)0x00, 1, 100);
  HAL_Delay(100);
  HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, GYRO_CONFIG_REG, 1, (uint8_t*)0x00, 1, 100);
  HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, (uint8_t*)0x08, 1, 100);
  HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, INT_ENABLE_REG, 1, (uint8_t*)0x01, 1, 100);
  
  HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, WHO_AM_I_REG, 1, &res, 1, 100);
  return (res == 0x68) ? 0 : 1;
}

void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *az) {
  uint8_t buf[4];
  HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, buf, 4, 100);
  *ax = (int16_t)(buf[0] << 8 | buf[1]) - accel_offset_x;
  *az = (int16_t)(buf[2] << 8 | buf[3]) - accel_offset_z;
}

void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *gx) {
  uint8_t buf[2];
  HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, buf, 2, 100);
  *gx = (int16_t)(buf[0] << 8 | buf[1]);
}

void MPU6050_Calibrate(I2C_HandleTypeDef *hi2c) {
  int16_t ax, az;
  uint32_t sum_ax = 0, sum_az = 0;
  
  OLED_ShowString(0, 0, "Calibrating...");
  
  for (uint8_t i = 0; i < 100; i++) {
    MPU6050_ReadAccel(hi2c, &ax, &az);
    sum_ax += ax;
    sum_az += az;
    HAL_Delay(10);
  }
  
  accel_offset_x = (float)sum_ax / 100.0f;
  accel_offset_z = (float)sum_az / 100.0f - 8192.0f;
  
  FLASH_Unlock();
  FLASH_ProgramWord(0x08080000, *(uint32_t*)&accel_offset_x);
  FLASH_ProgramWord(0x08080004, *(uint32_t*)&accel_offset_z);
  FLASH_Lock();
  
  OLED_Clear();
  OLED_ShowString(0, 0, "Calibrate OK");
  HAL_Delay(1000);
}

void Calculate_Angle(I2C_HandleTypeDef *hi2c) {
  int16_t ax, az, gx;
  float accel_angle, gyro_rate;
  
  MPU6050_ReadAccel(hi2c, &ax, &az);
  MPU6050_ReadGyro(hi2c, &gx);
  
  accel_angle = atan2f((float)ax, (float)az) * 180.0f / PI;
  gyro_rate = (float)gx / 131.0f;
  
  angle = 0.96f * (angle + gyro_rate * 0.1f) + 0.04f * accel_angle;
}
