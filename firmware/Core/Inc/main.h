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
#define UCPD2_VBUS_DISCHG_Pin GPIO_PIN_13
#define UCPD2_VBUS_DISCHG_GPIO_Port GPIOC
#define DCDC2_EN_Pin GPIO_PIN_14
#define DCDC2_EN_GPIO_Port GPIOC
#define NRST_Pin GPIO_PIN_2
#define NRST_GPIO_Port GPIOF
#define VIN_Pin GPIO_PIN_0
#define VIN_GPIO_Port GPIOA
#define VBUS1_Pin GPIO_PIN_1
#define VBUS1_GPIO_Port GPIOA
#define VOUT1_Pin GPIO_PIN_2
#define VOUT1_GPIO_Port GPIOA
#define UCPD_ISENSE1_Pin GPIO_PIN_3
#define UCPD_ISENSE1_GPIO_Port GPIOA
#define VBUS2_Pin GPIO_PIN_4
#define VBUS2_GPIO_Port GPIOA
#define VOUT2_Pin GPIO_PIN_5
#define VOUT2_GPIO_Port GPIOA
#define UCPD_ISENSE2_Pin GPIO_PIN_6
#define UCPD_ISENSE2_GPIO_Port GPIOA
#define UCPD1_PDO2_Pin GPIO_PIN_0
#define UCPD1_PDO2_GPIO_Port GPIOB
#define UCPD1_PDO3_Pin GPIO_PIN_1
#define UCPD1_PDO3_GPIO_Port GPIOB
#define UCPD1_PDO4_Pin GPIO_PIN_2
#define UCPD1_PDO4_GPIO_Port GPIOB
#define UCPD1_PDO5_Pin GPIO_PIN_10
#define UCPD1_PDO5_GPIO_Port GPIOB
#define UCPD1_VBUS_DISCHG_Pin GPIO_PIN_11
#define UCPD1_VBUS_DISCHG_GPIO_Port GPIOB
#define UCPD1_SRC_EN_Pin GPIO_PIN_12
#define UCPD1_SRC_EN_GPIO_Port GPIOB
#define VCONN1_EN_Pin GPIO_PIN_13
#define VCONN1_EN_GPIO_Port GPIOB
#define DCDC1_PGOOD_Pin GPIO_PIN_14
#define DCDC1_PGOOD_GPIO_Port GPIOB
#define VCONN1_SEL_Pin GPIO_PIN_9
#define VCONN1_SEL_GPIO_Port GPIOA
#define DCDC1_EN_Pin GPIO_PIN_10
#define DCDC1_EN_GPIO_Port GPIOA
#define DCDC2_PGOOD_Pin GPIO_PIN_11
#define DCDC2_PGOOD_GPIO_Port GPIOA
#define VCONN2_EN_Pin GPIO_PIN_12
#define VCONN2_EN_GPIO_Port GPIOA
#define UCPD2_PDO5_Pin GPIO_PIN_3
#define UCPD2_PDO5_GPIO_Port GPIOD
#define UCPD2_PDO4_Pin GPIO_PIN_3
#define UCPD2_PDO4_GPIO_Port GPIOB
#define UCPD2_PDO3_Pin GPIO_PIN_4
#define UCPD2_PDO3_GPIO_Port GPIOB
#define UCPD2_PDO2_Pin GPIO_PIN_5
#define UCPD2_PDO2_GPIO_Port GPIOB
#define UCPD2_SRC_EN_Pin GPIO_PIN_6
#define UCPD2_SRC_EN_GPIO_Port GPIOB
#define VCONN2_SEL_Pin GPIO_PIN_7
#define VCONN2_SEL_GPIO_Port GPIOB
#define USER_LED_Pin GPIO_PIN_8
#define USER_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
