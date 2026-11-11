#include "FreeRTOSWrapper.hpp"
#include "NEO_M9N/NEO_M9N.hpp"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "usart.h"
#include "stream_buffer.h"
#include <cstring>
#include <cstdio>

#include "Display/Inc/DisplayController.hpp"
#include "Display/Inc/Terminal/Terminal.hpp"
#include "Delta/Inc/TrackSession.hpp"

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

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dmaBufferGpsRx, DMA_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);

    uint8_t gpsData[128];

    TrackSession track_session;
    track_session.start();

    for(;;)
    {
        auto messageSize = xStreamBufferReceive(xStreamBufferGps, gpsData, X_BUFFER_SIZE, portMAX_DELAY);

        UBX_NAV_PVT payload;
        if(UBXFrameDecode(gpsData, payload.data) == 0)
        {
            terminal0.clear();
            track_session.processData(
                payload.pvt.lat,
                payload.pvt.lon,
                payload.pvt.height,
                payload.pvt.velN,
                payload.pvt.velE,
                payload.pvt.headMot,
                payload.pvt.fixType,
                payload.pvt.gnssFixOK && !payload.pvt.invalidLlh,
                payload.pvt.sAcc
            );
            terminal0.printf("Delta [ms]: %d\n", track_session.getDelta());
            terminal0.printf("Velocity [mm^2/s^2]: %d\n", (payload.pvt.velN * payload.pvt.velE));
            terminal0.printf("Lat [deg]: %d Lon [deg]: %d\n", payload.pvt.lat, payload.pvt.lon);
            terminal0.printf("Fix:  %d %d\n", payload.pvt.fixType, payload.pvt.fixType);
            HAL_UART_Transmit_DMA(&huart2, gpsData, messageSize);
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
