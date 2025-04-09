#include "sensors.h"
#include "delay.h"  // 需实现微秒级延时函数

#define DS18B20_PIN    GPIO_Pin_3
#define DS18B20_PORT   GPIOA

// 初始化GPIO
void DS18B20_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

// 读取温度值（简化版，需根据DS18B20时序实现）
float Sensors_ReadTemperature(void) {
    // 发送复位脉冲
    DS18B20_WriteByte(0xCC);  // 跳过ROM
    DS18B20_WriteByte(0x44);  // 启动温度转换
    delay_ms(750);            // 等待转换完成
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);  // 读取暂存器
    
    uint8_t temp_l = DS18B20_ReadByte();
    uint8_t temp_h = DS18B20_ReadByte();
    int16_t temp_raw = (temp_h << 8) | temp_l;
    
    return temp_raw / 16.0;
}
