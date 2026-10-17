#include "FreeRTOSWrapper.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

void App_MainTask(void *arg)
{
    for(;;)
    {
        osDelay(1);
    }
}