#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "main.h"

#define MPU6050_ADDR      0x68  // AD0 接地时为 0x68

/* MPU6050 寄存器地址 */
#define MPU6050_PWR_MGMT_1     0x6B
#define MPU6050_ACCEL_XOUT_H   0x3B
#define MPU6050_GYRO_XOUT_H    0x43
#define MPU6050_ACCEL_CONFIG   0x1C
#define MPU6050_GYRO_CONFIG    0x1B

/* 姿态角结构体 */
typedef struct {
    float pitch;    // 俯仰角（°）
    float roll;     // 横滚角（°）
} mpu6050_angle_t;

/* 加速度原始数据 */
typedef struct {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
} mpu6050_accel_t;

void MPU6050_Init(void);
HAL_StatusTypeDef MPU6050_ReadAccel(mpu6050_accel_t *accel);
HAL_StatusTypeDef MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);
void MPU6050_ComputeAngle(mpu6050_accel_t *accel, mpu6050_angle_t *angle);
uint8_t MPU6050_DetectFreeFall(mpu6050_accel_t *accel, float threshold_g);

#endif