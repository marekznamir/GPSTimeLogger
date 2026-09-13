#include "freertosWrapper.h"
#include "DisplayController.hpp"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "main.h"
#include "spi.h"
#include "stm32h7a3xx.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_tim.h"
#include <cstdint>
#include <cstdio>
#include "string.h"

#define KEYBOARD_WAKE_FLAG (1U << 0)

DisplayController displayController;
extern TIM_HandleTypeDef htim2;


u8g2_t u8g2;
bool initOnce = true;

void displayTask()
{
    if(initOnce)
    {
        displayController.init();
        initOnce = false;

        HAL_GPIO_WritePin(
        GPIOB,
        LED0_Pin | LED1_Pin | LED2_Pin |
        LED3_Pin | LED4_Pin | LED5_Pin,
        GPIO_PIN_SET
        );

        HAL_GPIO_WritePin(
            GPIOC,
            COM0_Pin | COM1_Pin | COM2_Pin,
            GPIO_PIN_RESET
        );

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

        HAL_GPIO_WritePin(GPS_RST_GPIO_Port, GPS_RST_Pin, GPIO_PIN_SET);
        //osDelay(1000);
    }

    for(;;)
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, displayController.getBrightness());
        displayController.render();
        osDelay(50);
    }
}

void setOneComHight(uint8_t index, GPIO_TypeDef** COM_PORTS, uint16_t* COM_PINS, uint8_t COM_SIZE)
{
    for(uint8_t i = 0; i < COM_SIZE; i++)
    {
        if(i == index)
        {
            HAL_GPIO_WritePin(COM_PORTS[i], COM_PINS[i], GPIO_PIN_SET);
        }
        else 
        {
            HAL_GPIO_WritePin(COM_PORTS[i], COM_PINS[i], GPIO_PIN_RESET);
        }
    }
}

void delay_us(uint32_t us) 
{
    uint32_t start_ticks = DWT->CYCCNT;
    uint32_t target_ticks = us * (SystemCoreClock / 1000000U);
    
    while ((DWT->CYCCNT - start_ticks) < target_ticks);
}

void keyboardTask()
{
    HAL_GPIO_WritePin(
        GPIOB,
        LED0_Pin | LED1_Pin | LED2_Pin |
        LED3_Pin | LED4_Pin | LED5_Pin,
        GPIO_PIN_SET
    );

    HAL_GPIO_WritePin(
        GPIOC,
        COM0_Pin | COM1_Pin | COM2_Pin,
        GPIO_PIN_SET
    );

    GPIO_TypeDef* comPorts[] = {
        COM0_GPIO_Port,
        COM1_GPIO_Port,
        COM2_GPIO_Port
    };

    uint16_t comPins[] = {
        COM0_Pin,
        COM1_Pin,
        COM2_Pin
    };

    const uint16_t allLedPins =
        LED0_Pin | LED1_Pin | LED2_Pin |
        LED3_Pin | LED4_Pin | LED5_Pin;

    const uint16_t allComPins =
        COM0_Pin | COM1_Pin | COM2_Pin;

    // LED-y wyłączone
    HAL_GPIO_WritePin(GPIOB, allLedPins, GPIO_PIN_SET);

    // Wszystkie klawisze mogą wywołać EXTI
    HAL_GPIO_WritePin(GPIOC, allComPins, GPIO_PIN_SET);

    for (;;)
    {
        osThreadFlagsWait(
            KEYBOARD_WAKE_FLAG,
            osFlagsWaitAny,
            osWaitForever
        );

        HAL_NVIC_DisableIRQ(EXTI3_IRQn);
        HAL_NVIC_DisableIRQ(EXTI4_IRQn);

        osDelay(20);

        setOneComHight(0, comPorts, comPins, 3);
        delay_us(5);

        if (HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin) == GPIO_PIN_RESET)
            displayController.buttonDown();

        if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
            displayController.buttonUp();

        setOneComHight(1, comPorts, comPins, 3);
        delay_us(5);

        if (HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin) == GPIO_PIN_RESET)
            displayController.buttonBack();

        if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
            displayController.buttonOk();

        setOneComHight(2, comPorts, comPins, 3);
        delay_us(5);

        if (HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin) == GPIO_PIN_RESET)
            displayController.buttonFn();

        if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
            displayController.buttonView();

        // Stan oczekiwania: wszystkie kolumny zwarte do GND
        HAL_GPIO_WritePin(GPIOC, allComPins, GPIO_PIN_SET);

        // Poczekaj na puszczenie klawisza
        while (
            HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin) == GPIO_PIN_RESET ||
            HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET
        )
        {
            osDelay(10);
        }

        __HAL_GPIO_EXTI_CLEAR_IT(SW0_Pin | SW1_Pin);
        osThreadFlagsClear(KEYBOARD_WAKE_FLAG);

        HAL_NVIC_EnableIRQ(EXTI3_IRQn);
        HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    }
}

void NEOM9NTask()
{
static const char test[] = "USART2 TEST OK\r\n";

    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)test,
        sizeof(test) - 1,
        HAL_MAX_DELAY
    );

    HAL_GPIO_WritePin(
        GPS_RST_GPIO_Port,
        GPS_RST_Pin,
        GPIO_PIN_SET
    );

    osDelay(10);

    GpsFrame_t frame;

    if (HAL_UARTEx_ReceiveToIdle_DMA(
            &huart1,
            gpsDmaBuffer,
            sizeof(gpsDmaBuffer)) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_DMA_DISABLE_IT(
        huart1.hdmarx,
        DMA_IT_HT
    );

    for (;;)
    {
        if (osMessageQueueGet(
                gpsQueue,
                &frame,
                NULL,
                osWaitForever) == osOK)
        {

            char* data = frame.data;

            char lat_val[20] = {0};
            char lat_dir[2] = {0};
            char lon_val[20] = {0};
            char lon_dir[2] = {0};
            char fix[2] = {0};
            char numOfSatelites[4] = {0};

            if (strncmp(data, "$GNGGA", 6) == 0) 
            {
                uint16_t commaIndex = 0;
                char* startPtr = data;

                for (int j = 0; data[j] != '\0'; j++) 
                {
                    if (data[j] == ',' || data[j] == '*')
                    {
                        data[j] = '\0'; 
                        
                        switch (commaIndex) 
                        {
                            case 2: strncpy(lat_val, startPtr, sizeof(lat_val)-1); break;
                            case 3: strncpy(lat_dir, startPtr, sizeof(lat_dir)-1); break;
                            case 4: strncpy(lon_val, startPtr, sizeof(lon_val)-1); break;
                            case 5: strncpy(lon_dir, startPtr, sizeof(lon_dir)-1); break;
                            case 6: strncpy(fix, startPtr, sizeof(fix)-1); break;
                            case 7: strncpy(numOfSatelites, startPtr, sizeof(numOfSatelites)-1); break;
                        }
                        
                        commaIndex++;
                        startPtr = &data[j + 1];
                    }
                }

                char full_lat[32] = {0};
                char full_lon[32] = {0};

                strcpy(full_lat, lat_val);
                strcpy(full_lon, lon_val);

                strcat(full_lat, lat_dir);
                strcat(full_lon, lon_dir);

                displayController.setGPSData(full_lon, full_lat, fix, numOfSatelites);
            }    
            
            HAL_UART_Transmit(
                &huart2,
                (uint8_t *)frame.data,
                frame.length,
                100
            );
        }
    }
}