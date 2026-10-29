#include "FreeRTOSWrapper.hpp"
#include "NEO_M9N/NEO_M9N.hpp"
#include "portmacro.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "usart.h"
#include "stream_buffer.h"
#include <cstring>
#include <cstdio>

#include "Display/Inc/DisplayController.hpp"
#include "Display/Inc/Terminal/Terminal.hpp"

namespace cmd = Command;

uint8_t dmaBufferGpsRx[DMA_BUFFER_SIZE];
uint8_t dmaBufferClientRx[DMA_BUFFER_SIZE];

StreamBufferHandle_t xStreamBufferGps = nullptr;
StreamBufferHandle_t xStreamBufferClient = nullptr;

// importante

bool IS_IN_CFG_MODE = false; // temp val, dla trybu configure

// comprende?

cmd::Terminal terminal0;
static DisplayController displayController;

void App_MainTask(void *arg)
{
    displayController.init();

    if(IS_IN_CFG_MODE)
    {
        osThreadFlagsSet(CfgHandle, FLAG_START_CFG);    
    }

    for(;;)
    {
        displayController.draw();
        osDelay(50);
    }
}

void App_GpsTask(void *arg)
{
    xStreamBufferGps = xStreamBufferCreate(X_BUFFER_SIZE, X_BUFFER_TRIGGER_SIZE);

    // __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF |
    //                                UART_CLEAR_PEF  | UART_CLEAR_IDLEF);
    // __HAL_UART_SEND_REQ(&huart1, UART_RXDATA_FLUSH_REQUEST);
    // huart1.ErrorCode = HAL_UART_ERROR_NONE;

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dmaBufferGpsRx, DMA_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);

    uint8_t gpsData[128];

    for(;;)
    {
        auto messageSize = xStreamBufferReceive(xStreamBufferGps, gpsData, X_BUFFER_SIZE, portMAX_DELAY);

        UBX_NAV_PVT payload;
        if(UBXFrameDecode(gpsData, payload.data) == 0)
        {
            cmd::write(payload.pvt.month, terminal0);
        }
        osDelay(1);
    }
}

void App_CfgTask(void *arg)
{
    osThreadFlagsWait(FLAG_START_CFG, osFlagsWaitAny, osWaitForever);

    xStreamBufferClient = xStreamBufferCreate(X_BUFFER_SIZE, X_BUFFER_TRIGGER_SIZE);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dmaBufferClientRx, DMA_BUFFER_SIZE);

    uint8_t clientData[128];

    for(;;)
    {
        auto messageSize = xStreamBufferReceive(xStreamBufferClient, clientData, X_BUFFER_SIZE, portMAX_DELAY);

        //HAL_UART_Transmit(&huart2, clientData, messageSize, 100); // instant response with same message on port

        HAL_UART_Transmit(&huart1, clientData, messageSize, 100);

        osDelay(1);
    }
    
}