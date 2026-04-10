#ifndef __BSP_LORA_H
#define __BSP_LORA_H

#include "main.h"

#define LORA_MODE_TRANSPARENT  GPIO_PIN_RESET
#define LORA_MODE_AT           GPIO_PIN_SET

void LoRa_Init(void);
void LoRa_SetMode(uint8_t mode);
void LoRa_SendData(uint8_t *data, uint16_t len);
void LoRa_SendATCommand(char *cmd, char *response, uint16_t timeout);

#endif