#include "Callbacks.h"
#include "NEO_M9N/NEO_M9N.hpp"
#include "FreeRTOSWrapper.hpp"

#include "usart.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        NEO_M9N_RxEventCallback(Size);
    }
    else if(huart->Instance == USART2)
    {
        CLIENT_RxEventCallback(Size);
    }
}

