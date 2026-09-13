/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "freertosWrapper.h"
#include "string.h"
//#include "DisplayController.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYBOARD_WAKE_FLAG (1U << 0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern SPI_HandleTypeDef hspi1;

osSemaphoreId_t spiSem_;

osMessageQueueId_t gpsQueue = NULL;

__attribute__((section(".dma_buffer"), aligned(32)))
uint8_t gpsDmaBuffer[GPS_DMA_BUFFER_SIZE];

static char gpsAssemblyBuffer[GPS_FRAME_SIZE];
static uint16_t gpsAssemblyLength = 0;

volatile uint32_t gpsRxEventCount = 0;
volatile uint16_t gpsRxLastSize = 0;
volatile uint8_t gpsFirstByte = 0;

static uint8_t gpsProcessingBuffer[GPS_DMA_BUFFER_SIZE];

volatile HAL_StatusTypeDef gpsRearmStatus = HAL_OK;
volatile uint32_t gpsRearmErrorCount = 0;
volatile uint32_t gpsUartError = 0;
volatile uint32_t gpsDmaError = 0;
/* USER CODE END Variables */
/* Definitions for Display */
osThreadId_t DisplayHandle;
const osThreadAttr_t Display_attributes = {
  .name = "Display",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Keyboard */
osThreadId_t KeyboardHandle;
const osThreadAttr_t Keyboard_attributes = {
  .name = "Keyboard",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for NEO_M9N */
osThreadId_t NEO_M9NHandle;
const osThreadAttr_t NEO_M9N_attributes = {
  .name = "NEO_M9N",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void GPS_ProcessReceivedBytes(
    const uint8_t *data,
    uint16_t size)
{
    for (uint16_t i = 0; i < size; ++i)
    {
        char character = (char)data[i];

        if (character == '$')
        {
            gpsAssemblyLength = 0;
        }

        if (gpsAssemblyLength < (GPS_FRAME_SIZE - 1))
        {
            gpsAssemblyBuffer[gpsAssemblyLength++] = character;
        }
        else
        {
            /* Ramka za długa — odrzucamy ją */
            gpsAssemblyLength = 0;
            continue;
        }

        if (character == '\n')
        {
            GpsFrame_t frame;

            frame.length = gpsAssemblyLength;

            memcpy(
                frame.data,
                gpsAssemblyBuffer,
                gpsAssemblyLength
            );
            
            frame.data[frame.length] = '\0';

            osMessageQueuePut(
                gpsQueue,
                &frame,
                0,
                0
            );

            gpsAssemblyLength = 0;
        }
    }
}
/* USER CODE END FunctionPrototypes */

void StartDisplayTask(void *argument);
void StartKeyboardTask(void *argument);
void StartNEOM9NTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  gpsQueue = osMessageQueueNew(
    GPS_QUEUE_LENGTH,
    sizeof(GpsFrame_t),
    NULL
);

if (gpsQueue == NULL)
{
    Error_Handler();
}
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  spiSem_ = osSemaphoreNew(1, 0, NULL);


  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Display */
  DisplayHandle = osThreadNew(StartDisplayTask, NULL, &Display_attributes);

  /* creation of Keyboard */
  KeyboardHandle = osThreadNew(StartKeyboardTask, NULL, &Keyboard_attributes);

  /* creation of NEO_M9N */
  NEO_M9NHandle = osThreadNew(StartNEOM9NTask, NULL, &NEO_M9N_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
  * @brief  Function implementing the Display thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDisplayTask */
  /* Infinite loop */
  // for(;;)
  // {
  //   osDelay(1);
  // }
  displayTask();

  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartKeyboardTask */
/**
* @brief Function implementing the Keyboard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyboardTask */
void StartKeyboardTask(void *argument)
{
  /* USER CODE BEGIN StartKeyboardTask */
  /* Infinite loop */
  keyboardTask();
  /* USER CODE END StartKeyboardTask */
}

/* USER CODE BEGIN Header_StartNEOM9NTask */
/**
* @brief Function implementing the NEO_M9N thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNEOM9NTask */
void StartNEOM9NTask(void *argument)
{
  /* USER CODE BEGIN StartNEOM9NTask */
  /* Infinite loop */
  NEOM9NTask();
  /* USER CODE END StartNEOM9NTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        osSemaphoreRelease(spiSem_);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if ((GPIO_Pin == SW0_Pin) || (GPIO_Pin == SW1_Pin))
    {
        if (KeyboardHandle != NULL)
        {
            osThreadFlagsSet(KeyboardHandle, KEYBOARD_WAKE_FLAG);
        }
    }
}

void HAL_UARTEx_RxEventCallback(
    UART_HandleTypeDef *huart,
    uint16_t size)
{
    if (huart->Instance != USART1)
    {
        return;
    }

    gpsRxEventCount++;
    gpsRxLastSize = size;

    if (size > GPS_DMA_BUFFER_SIZE)
    {
        size = GPS_DMA_BUFFER_SIZE;
    }

    /*
     * Zachowujemy dane, zanim gpsDmaBuffer ponownie
     * zostanie przekazany do DMA.
     */
    memcpy(
        gpsProcessingBuffer,
        gpsDmaBuffer,
        size
    );

    /*
     * Usuń potencjalny overrun i natychmiast wznów DMA.
     */
    __HAL_UART_CLEAR_OREFLAG(&huart1);

    gpsRearmStatus = HAL_UARTEx_ReceiveToIdle_DMA(
        &huart1,
        gpsDmaBuffer,
        sizeof(gpsDmaBuffer)
    );

    gpsUartError = huart1.ErrorCode;
    gpsDmaError = huart1.hdmarx->ErrorCode;

    if (gpsRearmStatus == HAL_OK)
    {
        __HAL_DMA_DISABLE_IT(
            huart1.hdmarx,
            DMA_IT_HT
        );
    }
    else
    {
        gpsRearmErrorCount++;
    }

    /*
     * Dopiero po ponownym uzbrojeniu DMA parsujemy kopię.
     */
    GPS_ProcessReceivedBytes(
        gpsProcessingBuffer,
        size
    );
}
/* USER CODE END Application */

