#include "freertosWrapper.h"
#include "DisplayController.hpp"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "spi.h"

DisplayController displayController;
u8g2_t u8g2;
bool initOnce = true;

void displayTask()
{
    if(initOnce)
    {
        displayController.init();
        initOnce = false;
    }

    for(;;)
    {
        displayController.render();
        osDelay(50);
    }
}