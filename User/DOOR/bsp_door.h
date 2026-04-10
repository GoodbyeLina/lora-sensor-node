#ifndef __BSP_DOOR_H
#define __BSP_DOOR_H

#include "main.h"

typedef enum {
    DOOR_CLOSED = 0,
    DOOR_OPEN   = 1
} door_state_t;

void Door_Init(void);
door_state_t Door_GetState(void);
void Door_SetCallback(void (*cb)(door_state_t state));  // 注册回调函数

#endif
