#include "stm32f10x.h"
#include "adc.h"
#include "uart.h"
#include "sensors.h"

// 定义测量通道
#define VOLTAGE_CHANNEL    ADC_Channel_0  // PA0
#define TEMP_CHANNEL       ADC_Channel_1  // PA1
#define PRESSURE_CHANNEL   ADC_Channel_2  // PA2

int main(void) {
    // 初始化系统时钟
    SystemInit();
    
    // 硬件初始化
    ADC_Init();
    UART_Init(115200);  // 串口波特率115200
    Sensors_Init();
    
    while(1) {
        // 每1秒采集一次数据
        float voltage = ADC_ReadVoltage(VOLTAGE_CHANNEL);
        float temp = Sensors_ReadTemperature();
        float pressure = ADC_ReadPressure(PRESSURE_CHANNEL);
        
        // 通过串口发送数据
        UART_SendData(voltage, temp, pressure);
        
        // 延时1秒（简化实现，实际建议用定时器）
        for(int i=0; i<1000000; i++);
    }
}
