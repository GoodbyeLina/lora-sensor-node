#ifndef __BSP_SHT30_H
#define __BSP_SHT30_H

#include "main.h"

#define SHT30_ADDR        0x44  // SHT30 默认 I2C 地址（ADDR 引脚接地）

/* 测量模式命令（高重复性，时钟拉伸使能） */
#define SHT30_CMD_MEAS_HIGH_REP  0x2C06
#define SHT30_CMD_MEAS_MED_REP   0x2C0D
#define SHT30_CMD_MEAS_LOW_REP   0x2C10

typedef struct {
    float temperature;  // 温度（℃）
    float humidity;     // 湿度（%RH）
} sht30_data_t;

HAL_StatusTypeDef SHT30_Init(void);
HAL_StatusTypeDef SHT30_ReadData(sht30_data_t *data);
uint8_t SHT30_CRC8(const uint8_t *data, uint8_t len);

#endif
