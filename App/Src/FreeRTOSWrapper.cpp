#include "FreeRTOSWrapper.hpp"

#include "portmacro.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "usart.h"
#include "stream_buffer.h"
#include <cstring>

uint8_t dmaBuffer[DMA_BUFFER_SIZE];
StreamBufferHandle_t xStreamBuffer = nullptr;

// importante

bool IS_IN_CFG_MODE = true; // temp val, dla trybu configure

// comprende?

void App_MainTask(void *arg)
{
    
    if(IS_IN_CFG_MODE)
    {
        osThreadFlagsSet(CfgHandle, FLAG_START_CFG);
    }

    for(;;)
    {
        osDelay(50);
    }
}

void App_GpsTask(void *arg)
{
    xStreamBuffer = xStreamBufferCreate(X_BUFFER_SIZE, X_BUFFER_TRIGGER_SIZE);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dmaBuffer, DMA_BUFFER_SIZE);

    uint8_t gpsData[128];

    for(;;)
    {
        xStreamBufferReceive(xStreamBuffer, gpsData, X_BUFFER_SIZE, portMAX_DELAY);

        HAL_UART_Transmit(&huart2, gpsData, 128, 100);

        osDelay(1);
    }
}

void App_CfgTask(void *arg)
{
    osThreadFlagsWait(FLAG_START_CFG, osFlagsWaitAny, osWaitForever);

    for(;;)
    {
        osDelay(1);
    }
    
}