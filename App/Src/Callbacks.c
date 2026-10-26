#include "Callbacks.h"
#include "NEO_M9N/NEO_M9N.hpp"
#include "Display/Inc/Callbacks.hpp"
#include "FreeRTOSWrapper.hpp"

#include "usart.h"
#include "spi.h"

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

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        osSemaphoreRelease(spiSemHandle);
    }
}
