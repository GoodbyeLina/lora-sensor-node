#include "bsp_door.h"
#include "gpio.h"
#include <stdio.h>

static void (*door_callback)(door_state_t state) = NULL;

void Door_Init(void) {
    // EXTI 已在 CubeMX 中配置，此处只需确保中断使能
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

door_state_t Door_GetState(void) {
    return (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) ? 
            DOOR_CLOSED : DOOR_OPEN;
}

void Door_SetCallback(void (*cb)(door_state_t state)) {
    door_callback = cb;
}

/* EXTI 中断回调 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_12) {
        door_state_t state = Door_GetState();
        if (door_callback) {
            door_callback(state);
        }
        // 打印状态变化（便于调试）
        printf("[DOOR] State changed: %s\r\n", state ? "OPEN" : "CLOSED");
    }
}
