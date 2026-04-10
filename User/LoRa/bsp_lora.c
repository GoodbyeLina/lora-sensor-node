#include "bsp_lora.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

static void LoRa_WaitReady(void) {
    while(HAL_GPIO_ReadPin(LORA_AUX_GPIO_Port, LORA_AUX_Pin) == GPIO_PIN_RESET);
}

void LoRa_Init(void) {
    LoRa_SetMode(LORA_MODE_TRANSPARENT);
    HAL_Delay(100);
    printf("[LoRa] Init OK\r\n");
}

void LoRa_SetMode(uint8_t mode) {
    HAL_GPIO_WritePin(LORA_MD0_GPIO_Port, LORA_MD0_Pin, mode);
    HAL_Delay(20);
}

void LoRa_SendData(uint8_t *data, uint16_t len) {
    LoRa_SetMode(LORA_MODE_TRANSPARENT);
    LoRa_WaitReady();
    HAL_UART_Transmit(&huart1, data, len, 1000);
}

void LoRa_SendATCommand(char *cmd, char *response, uint16_t timeout) {
    char rx_buffer[64] = {0};
    uint8_t rx_index = 0;
    uint32_t start_tick = HAL_GetTick();

    LoRa_SetMode(LORA_MODE_AT);
    HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), 1000);
    HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 1000);

    while(HAL_GetTick() - start_tick < timeout) {
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
            char ch = (char)(huart1.Instance->DR & 0xFF);
            if(ch != '\r' && ch != '\n' && rx_index < 63) {
                rx_buffer[rx_index++] = ch;
            }
            if(strstr(rx_buffer, "OK") != NULL) {
                break;
            }
            start_tick = HAL_GetTick();
        }
    }
    rx_buffer[rx_index] = '\0';
    if(strlen(rx_buffer) > 0) {
        printf("AT Resp: %s\r\n", rx_buffer);
        if(response != NULL) {
            strcpy(response, rx_buffer);
        }
    }
    LoRa_SetMode(LORA_MODE_TRANSPARENT);
}