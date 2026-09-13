#include "cmsis_os2.h"
#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif

extern osSemaphoreId_t spiSem_;

#define GPS_DMA_BUFFER_SIZE 256
#define GPS_FRAME_SIZE      128
#define GPS_QUEUE_LENGTH    16

typedef struct
{
    uint16_t length;
    char data[GPS_FRAME_SIZE];
} GpsFrame_t;

extern osSemaphoreId_t spiSem_;
extern osMessageQueueId_t gpsQueue;
extern uint8_t gpsDmaBuffer[GPS_DMA_BUFFER_SIZE];

void displayTask(void);

void keyboardTask(void);

void NEOM9NTask(void);

#ifdef __cplusplus
}
#endif