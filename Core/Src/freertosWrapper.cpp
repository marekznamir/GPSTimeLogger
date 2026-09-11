#include "freertosWrapper.h"
#include "DisplayController.hpp"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

DisplayController displayController;

void displayTask()
{
    for(;;)
    {
        displayController.render();
        osDelay(200);
    }
}