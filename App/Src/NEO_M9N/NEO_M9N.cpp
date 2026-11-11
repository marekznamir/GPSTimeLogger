#include "NEO_M9N/NEO_M9N.hpp"
#include "FreeRTOSWrapper.hpp"

#include "portmacro.h"
#include "stream_buffer.h"
#include "usart.h"
#include <cstring>

void NEO_M9N_RxEventCallback(uint16_t size)
{
    if(xStreamBufferGps != nullptr)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE; // zwrotka czy wywłaszczy taska 

        xStreamBufferSendFromISR(xStreamBufferGps, dmaBufferGpsRx, size, &xHigherPriorityTaskWoken);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dmaBufferGpsRx, DMA_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT); // do sprawdzenia czy cos zmienia
    }
}

void CLIENT_RxEventCallback(uint16_t size)
{
    if(xStreamBufferClient != nullptr)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xStreamBufferSendFromISR(xStreamBufferClient, dmaBufferClientRx, size, &xHigherPriorityTaskWoken);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dmaBufferClientRx, DMA_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    }
}

/*
Add to UBXFrameDecode enum class as return value 
*/

[[nodiscard]] uint8_t UBXFrameDecode(uint8_t* frame, uint8_t* payload) 
{
    constexpr uint8_t CLASS_INDEX = 2;
    constexpr uint8_t ID_INDEX = 3;
    constexpr uint16_t PAYLOAD_START_INDEX = 6;

    const uint8_t& message_class = frame[CLASS_INDEX];
    const uint8_t& messsage_id = frame[ID_INDEX];
    uint16_t pLen = (static_cast<uint16_t>(frame[5]) << 8) | frame[4];

    uint8_t ck_a = 0;
    uint8_t ck_b = 0;
    uint16_t payload_end_index = PAYLOAD_START_INDEX + pLen;

    for (uint16_t i = 2; i < payload_end_index; i++) 
    {
        ck_a += frame[i];
        ck_b += ck_a;
    }

    const uint8_t& frame_ck_a = frame[6 + pLen];
    const uint8_t& frame_ck_b = frame[7 + pLen];

    if(ck_a != frame_ck_a || ck_b != frame_ck_b)
    {
        return 1; // checksum not ok
    }

    if(message_class == 0x01 && messsage_id == 0x07)
    {
        std::memcpy(payload, &frame[PAYLOAD_START_INDEX], pLen);
        return 0;
    }




    return 2;
}
