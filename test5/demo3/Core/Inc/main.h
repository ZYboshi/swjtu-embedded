/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
extern volatile uint8_t seg_buf[];
/*´®¿Ú*/
extern uint8_t receiveData[];
extern uint8_t transmitData[];

extern volatile uint8_t kb1_counter ;
extern volatile uint8_t kb2_counter ;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KB1_Pin GPIO_PIN_0
#define KB1_GPIO_Port GPIOE
#define KB1_EXTI_IRQn EXTI0_IRQn
#define KB2_Pin GPIO_PIN_1
#define KB2_GPIO_Port GPIOE
#define KB2_EXTI_IRQn EXTI1_IRQn
/* USER CODE BEGIN Private defines */
#define FRAME_SIZE 16

void BuildUartFrame(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
