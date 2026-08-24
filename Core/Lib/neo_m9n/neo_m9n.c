#include "neo_m9n.h"
#include "stm32h7xx_hal_uart.h"
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t* data;
    uint8_t len;
} QuickVec;

uint16_t create_payload(uint32_t key_id, uint8_t layers, uint32_t value, uint8_t value_size, uint8_t* payload)
{
    payload[0] = 0x00;
    payload[1] = layers; // 1 - RAM, 2 - BBR, 4 - FLASH, 7 - ALL
    payload[2] = 0x00; // reserved
    payload[3] = 0x00; // reserved

    payload[4] = (uint8_t)(key_id & 0xFF);
    payload[5] = (uint8_t)((key_id >> 8) & 0xFF);
    payload[6] = (uint8_t)((key_id >> 16) & 0xFF);
    payload[7] = (uint8_t)((key_id >> 24) & 0xFF);

    for (uint8_t i = 0; i < value_size; i++) 
    {
        payload[8 + i] = (uint8_t)((value >> (i * 8)) & 0xFF);
    }

    return 8 + value_size; // pLEN
}

uint16_t create_ubx_frame(uint8_t message_class, uint8_t message_id, uint8_t* pData, uint16_t pLen, uint8_t* frame_buffer) 
{
    frame_buffer[0] = UBX_SYNC_1;
    frame_buffer[1] = UBX_SYNC_2;
    frame_buffer[2] = message_class;
    frame_buffer[3] = message_id;
    
    frame_buffer[4] = (uint8_t)(pLen & 0xFF);
    frame_buffer[5] = (uint8_t)((pLen >> 8) & 0xFF);
    
    memcpy(&frame_buffer[6], pData, pLen);
    
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;
    uint16_t payload_end_index = 6 + pLen;
    
    for (uint16_t i = 2; i < payload_end_index; i++) {
        ck_a += frame_buffer[i];
        ck_b += ck_a;
    }
    
    frame_buffer[payload_end_index] = ck_a;
    frame_buffer[payload_end_index + 1] = ck_b;
    
    return 8 + pLen; // FRAME LEN
}

void neo_m9n_init(void)
{
    uint8_t payload_buff[32];
    uint8_t ubx_frame_buff[32];

    QuickVec payload;
    QuickVec frame;

    frame.data = ubx_frame_buff;
    payload.data = payload_buff;
    
    payload.len = create_payload(CFG_UART1OUTPROT_NMEA, 1, 0, 1, payload.data);
    frame.len = create_ubx_frame(UBX_CFG_VALSET_CLASS, UBX_CFG_VALSET_ID, frame.data, payload.len, frame.data);
    HAL_UART_Transmit_DMA(NEO_M9N_USARD_Handle, frame.data, frame.len);

    payload.len = create_payload(CFG_MSGOUT_UBX_NAV_PVT_UART1, 1, 1, 1, payload.data);
    frame.len = create_ubx_frame(UBX_CFG_VALSET_CLASS, UBX_CFG_VALSET_ID, frame.data, payload.len, frame.data);
    HAL_UART_Transmit_DMA(NEO_M9N_USARD_Handle, frame.data, frame.len);

    
}



