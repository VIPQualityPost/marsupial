/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_dma.h"

#include "stm32g0xx_ll_exti.h"

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
#define PDO1_Pin GPIO_PIN_5
#define PDO1_GPIO_Port GPIOC
#define PDO2_Pin GPIO_PIN_0
#define PDO2_GPIO_Port GPIOB
#define PDO3_Pin GPIO_PIN_1
#define PDO3_GPIO_Port GPIOB
#define PDO4_Pin GPIO_PIN_2
#define PDO4_GPIO_Port GPIOB
#define PDO5_Pin GPIO_PIN_10
#define PDO5_GPIO_Port GPIOB
#define Vbus2_DISCHG_Pin GPIO_PIN_11
#define Vbus2_DISCHG_GPIO_Port GPIOB
#define Vbus2_EN_Pin GPIO_PIN_12
#define Vbus2_EN_GPIO_Port GPIOB
#define Vbus_EN_Pin GPIO_PIN_4
#define Vbus_EN_GPIO_Port GPIOD
#define Vbus_DISCHG_Pin GPIO_PIN_5
#define Vbus_DISCHG_GPIO_Port GPIOD
#define PDO5D6_Pin GPIO_PIN_6
#define PDO5D6_GPIO_Port GPIOD
#define PDO4B3_Pin GPIO_PIN_3
#define PDO4B3_GPIO_Port GPIOB
#define PDO3B4_Pin GPIO_PIN_4
#define PDO3B4_GPIO_Port GPIOB
#define PDO2B5_Pin GPIO_PIN_5
#define PDO2B5_GPIO_Port GPIOB
#define PDO1B6_Pin GPIO_PIN_6
#define PDO1B6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
