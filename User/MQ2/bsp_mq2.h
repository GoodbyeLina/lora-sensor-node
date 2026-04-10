#ifndef __BSP_MQ2_H
#define __BSP_MQ2_H

#include "main.h"

void MQ2_Init(void);
uint32_t MQ2_GetADCValue(void);
float MQ2_GetVoltage(void);
uint8_t MQ2_GetAlarmState(void);

#endif
