#include "bsp_mq2.h"
#include "adc.h"

void MQ2_Init(void) {
    // ADC 已在 CubeMX 中初始化，这里可做校准
    HAL_ADCEx_Calibration_Start(&hadc1);
}

uint32_t MQ2_GetADCValue(void) {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
        return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}

float MQ2_GetVoltage(void) {
    return MQ2_GetADCValue() * 3.3f / 4095.0f;
}

uint8_t MQ2_GetAlarmState(void) {
    // 读取数字报警引脚状态（高电平表示浓度超限）
    return HAL_GPIO_ReadPin(MQ2_DO_GPIO_Port, MQ2_DO_Pin);
}
