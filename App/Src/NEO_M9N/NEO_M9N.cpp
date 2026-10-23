#include "NEO_M9N/NEO_M9N.hpp"
#include "FreeRTOSWrapper.hpp"

#include "portmacro.h"
#include "stream_buffer.h"
#include "usart.h"

void NEO_M9N_RxEventCallback(uint16_t size)
{
    if(xStreamBuffer != nullptr)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE; // zwrotka czy wywłaszczy taska 

        xStreamBufferSendFromISR(xStreamBuffer, dmaBuffer, size, &xHigherPriorityTaskWoken);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dmaBuffer, DMA_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT); // do sprawdzenia czy cos zmienia
    }
}
