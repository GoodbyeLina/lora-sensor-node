#include "bsp_sht30.h"
#include "i2c.h" 

/* CRC-8 校验（多项式 0x31，初始值 0xFF） */
uint8_t SHT30_CRC8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
        }
    }
    return crc;
}

/* 初始化 SHT30，设置为周期性测量模式 */
HAL_StatusTypeDef SHT30_Init(void) {
    uint16_t cmd = SHT30_CMD_MEAS_MED_REP; // 中等重复性，每秒测量一次
    uint8_t buf[2] = {cmd >> 8, cmd & 0xFF};
    return HAL_I2C_Master_Transmit(&hi2c1, SHT30_ADDR << 1, buf, 2, 100);
}

/* 读取温湿度数据 */
HAL_StatusTypeDef SHT30_ReadData(sht30_data_t *data) {
    uint8_t rx_buf[6];
    uint16_t temp_raw, hum_raw;

    // 读取 6 字节数据：温度高/低/CRC，湿度高/低/CRC
    if (HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR << 1, rx_buf, 6, 100) != HAL_OK) {
        return HAL_ERROR;
    }

    // CRC 校验
    if (SHT30_CRC8(rx_buf, 2) != rx_buf[2] ||
        SHT30_CRC8(rx_buf + 3, 2) != rx_buf[5]) {
        return HAL_ERROR;
    }

    // 转换为物理值（SHT30 公式）
    temp_raw = (rx_buf[0] << 8) | rx_buf[1];
    hum_raw  = (rx_buf[3] << 8) | rx_buf[4];
    data->temperature = -45.0f + 175.0f * temp_raw / 65535.0f;
    data->humidity    = 100.0f * hum_raw / 65535.0f;

    return HAL_OK;
}
