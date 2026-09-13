#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

extern osSemaphoreId_t spiSem_;

void displayTask(void);

void keyboardTask(void);

#ifdef __cplusplus
}
#endif