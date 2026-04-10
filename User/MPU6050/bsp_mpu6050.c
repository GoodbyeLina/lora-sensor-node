#include "bsp_mpu6050.h"
#include "i2c.h"
#include <math.h>
#include <stdio.h>

/* 内部函数：写寄存器 */
static HAL_StatusTypeDef MPU6050_WriteReg(uint8_t reg, uint8_t value) {
    return HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR << 1, reg, 1, &value, 1, 100);
}

/* 内部函数：读多个字节 */
static HAL_StatusTypeDef MPU6050_ReadData(uint8_t reg, uint8_t *data, uint8_t len) {
    return HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR << 1, reg, 1, data, len, 100);
}

/* 初始化 MPU6050 */
void MPU6050_Init(void) {
    // 唤醒 MPU6050（退出睡眠模式）
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x00);
    HAL_Delay(100);
    // 配置加速度计量程 ±2g
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x00);
    // 配置陀螺仪量程 ±250°/s（本例未使用，可忽略）
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x00);
    printf("[MPU6050] Init OK\r\n");
}

/* 读取加速度原始数据 */
HAL_StatusTypeDef MPU6050_ReadAccel(mpu6050_accel_t *accel) {
    uint8_t buf[6];
    if (MPU6050_ReadData(MPU6050_ACCEL_XOUT_H, buf, 6) != HAL_OK) {
        return HAL_ERROR;
    }
    accel->acc_x = (int16_t)((buf[0] << 8) | buf[1]);
    accel->acc_y = (int16_t)((buf[2] << 8) | buf[3]);
    accel->acc_z = (int16_t)((buf[4] << 8) | buf[5]);
    return HAL_OK;
}

/* 读取陀螺仪原始数据（可选，用于更精确的姿态解算） */
HAL_StatusTypeDef MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz) {
    uint8_t buf[6];
    if (MPU6050_ReadData(MPU6050_GYRO_XOUT_H, buf, 6) != HAL_OK) {
        return HAL_ERROR;
    }
    *gx = (int16_t)((buf[0] << 8) | buf[1]);
    *gy = (int16_t)((buf[2] << 8) | buf[3]);
    *gz = (int16_t)((buf[4] << 8) | buf[5]);
    return HAL_OK;
}

/* 根据加速度计计算俯仰角和横滚角（互补滤波可后续加入） */
void MPU6050_ComputeAngle(mpu6050_accel_t *accel, mpu6050_angle_t *angle) {
    // 将原始数据转换为 g 为单位（±2g 时灵敏度为 16384 LSB/g）
    float ax = accel->acc_x / 16384.0f;
    float ay = accel->acc_y / 16384.0f;
    float az = accel->acc_z / 16384.0f;

    // 计算俯仰角和横滚角（单位：弧度）
    angle->pitch = atan2(ay, sqrt(ax * ax + az * az)) * 180.0f / M_PI;
    angle->roll  = atan2(-ax, az) * 180.0f / M_PI;
}

/* 简易跌落检测：判断合加速度是否持续低于阈值 */
uint8_t MPU6050_DetectFreeFall(mpu6050_accel_t *accel, float threshold_g) {
    float ax = accel->acc_x / 16384.0f;
    float ay = accel->acc_y / 16384.0f;
    float az = accel->acc_z / 16384.0f;
    float norm = sqrt(ax*ax + ay*ay + az*az);
    
    // 如果合加速度小于阈值，认为处于失重状态（自由落体）
    if (norm < threshold_g) {
        return 1;
    }
    return 0;
}