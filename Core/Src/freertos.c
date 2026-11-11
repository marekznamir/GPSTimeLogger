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
#include "FreeRTOSWrapper.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Main */
osThreadId_t MainHandle;
uint32_t MainBuffer[ 1024 ];
osStaticThreadDef_t MainControlBlock;
const osThreadAttr_t Main_attributes = {
  .name = "Main",
  .cb_mem = &MainControlBlock,
  .cb_size = sizeof(MainControlBlock),
  .stack_mem = &MainBuffer[0],
  .stack_size = sizeof(MainBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Gps */
osThreadId_t GpsHandle;
uint32_t GpsBuffer[ 1024 ];
osStaticThreadDef_t GpsControlBlock;
const osThreadAttr_t Gps_attributes = {
  .name = "Gps",
  .cb_mem = &GpsControlBlock,
  .cb_size = sizeof(GpsControlBlock),
  .stack_mem = &GpsBuffer[0],
  .stack_size = sizeof(GpsBuffer),
  .priority = (osPriority_t) osPriorityBelowNormal7,
};
/* Definitions for Cfg */
osThreadId_t CfgHandle;
uint32_t ConfigureBuffer[ 1024 ];
osStaticThreadDef_t ConfigureControlBlock;
const osThreadAttr_t Cfg_attributes = {
  .name = "Cfg",
  .cb_mem = &ConfigureControlBlock,
  .cb_size = sizeof(ConfigureControlBlock),
  .stack_mem = &ConfigureBuffer[0],
  .stack_size = sizeof(ConfigureBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for spiSem */
osSemaphoreId_t spiSemHandle;
osStaticSemaphoreDef_t spiSemControlBlock;
const osSemaphoreAttr_t spiSem_attributes = {
  .name = "spiSem",
  .cb_mem = &spiSemControlBlock,
  .cb_size = sizeof(spiSemControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartMainTask(void *argument);
void StartGpsTask(void *argument);
void StartCfgTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of spiSem */
  spiSemHandle = osSemaphoreNew(1, 0, &spiSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Main */
  MainHandle = osThreadNew(StartMainTask, NULL, &Main_attributes);

  /* creation of Gps */
  GpsHandle = osThreadNew(StartGpsTask, NULL, &Gps_attributes);

  /* creation of Cfg */
  CfgHandle = osThreadNew(StartCfgTask, NULL, &Cfg_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartMainTask */
/**
  * @brief  Function implementing the Main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartMainTask */
  /* Infinite loop */
  
  App_MainTask(NULL);

  /* USER CODE END StartMainTask */
}

/* USER CODE BEGIN Header_StartGpsTask */
/**
* @brief Function implementing the Gps thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGpsTask */
void StartGpsTask(void *argument)
{
  /* USER CODE BEGIN StartGpsTask */
  /* Infinite loop */
  App_GpsTask(GpsHandle);
  /* USER CODE END StartGpsTask */
}

/* USER CODE BEGIN Header_StartCfgTask */
/**
* @brief Function implementing the Cfg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCfgTask */
void StartCfgTask(void *argument)
{
  /* USER CODE BEGIN StartCfgTask */
  /* Infinite loop */
  App_CfgTask(NULL);
  /* USER CODE END StartCfgTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

