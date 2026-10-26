#pragma once

#include "FreeRTOS.h"
#include <cmsis_os2.h>
#include "stream_buffer.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId_t MainHandle;
extern osThreadId_t GpsHandle;
extern osThreadId_t CfgHandle;

extern osSemaphoreId_t spiSemHandle;

#define FLAG_START_CFG (1U << 0)

#define X_BUFFER_SIZE 128 // bytes
#define X_BUFFER_TRIGGER_SIZE 1 // bytes

#define DMA_BUFFER_SIZE 128
extern uint8_t dmaBufferGpsRx[DMA_BUFFER_SIZE];
extern uint8_t dmaBufferClientRx[DMA_BUFFER_SIZE];

extern StreamBufferHandle_t xStreamBufferGps;
extern StreamBufferHandle_t xStreamBufferClient;

void App_MainTask(void *arg);

void App_GpsTask(void *arg);

void App_CfgTask(void *arg);

#ifdef __cplusplus
}
#endif