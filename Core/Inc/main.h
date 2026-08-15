/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPS_PPS_Pin GPIO_PIN_13
#define GPS_PPS_GPIO_Port GPIOC
#define GPS_EINT_Pin GPIO_PIN_14
#define GPS_EINT_GPIO_Port GPIOC
#define GPS_RST_Pin GPIO_PIN_15
#define GPS_RST_GPIO_Port GPIOC
#define COM0_Pin GPIO_PIN_0
#define COM0_GPIO_Port GPIOC
#define COM1_Pin GPIO_PIN_1
#define COM1_GPIO_Port GPIOC
#define COM2_Pin GPIO_PIN_2
#define COM2_GPIO_Port GPIOC
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOC
#define PWR_PUSH_Pin GPIO_PIN_0
#define PWR_PUSH_GPIO_Port GPIOA
#define DISP_DIMMER_Pin GPIO_PIN_1
#define DISP_DIMMER_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_3
#define VCP_RX_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_CD_Pin GPIO_PIN_6
#define LCD_CD_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOC
#define USB_PHY_INT_Pin GPIO_PIN_5
#define USB_PHY_INT_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOB
#define SD_D0_Pin GPIO_PIN_13
#define SD_D0_GPIO_Port GPIOB
#define GPS_TX_Pin GPIO_PIN_14
#define GPS_TX_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_15
#define GPS_RX_GPIO_Port GPIOB
#define CHARGER_INT_Pin GPIO_PIN_6
#define CHARGER_INT_GPIO_Port GPIOC
#define SD_DET_Pin GPIO_PIN_7
#define SD_DET_GPIO_Port GPIOC
#define SD_D1_Pin GPIO_PIN_9
#define SD_D1_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define PWR_KILL_Pin GPIO_PIN_9
#define PWR_KILL_GPIO_Port GPIOA
#define LCD_RST_Pin GPIO_PIN_10
#define LCD_RST_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SD_D2_Pin GPIO_PIN_10
#define SD_D2_GPIO_Port GPIOC
#define SD_D3_Pin GPIO_PIN_11
#define SD_D3_GPIO_Port GPIOC
#define SD_CLK_Pin GPIO_PIN_12
#define SD_CLK_GPIO_Port GPIOC
#define SD_CMD_Pin GPIO_PIN_2
#define SD_CMD_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_6
#define LED5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
