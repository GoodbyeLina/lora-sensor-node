#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>

#define NODE_ID  0x01  // 节点唯一 ID，可配置

#pragma pack(push, 1)
typedef struct {
    uint8_t header;         // 0xAA
    uint8_t node_id;
    uint8_t length;         // 固定 14
    int16_t temperature;    // 0.01℃
    uint16_t humidity;      // 0.01%
    uint16_t smoke_adc;     // 0~4095
    uint8_t door_state;     // 0=闭合, 1=打开
    uint8_t alarm_flags;    // bit0:烟雾, bit1:门磁, bit2:跌落
    int16_t pitch;          // 0.01°
    int16_t roll;           // 0.01°
    uint16_t crc16;
} sensor_frame_t;
#pragma pack(pop)

uint16_t CRC16_Calc(const uint8_t *data, uint16_t len);
void Protocol_SendFrame(sensor_frame_t *frame);

#endif