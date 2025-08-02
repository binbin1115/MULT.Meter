#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"

extern float accel_offset_x, accel_offset_z;
extern float angle;

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *az);
void MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *gx);
void MPU6050_Calibrate(I2C_HandleTypeDef *hi2c);
void Calculate_Angle(I2C_HandleTypeDef *hi2c);

#endif
