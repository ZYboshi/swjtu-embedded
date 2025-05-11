/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "basic.h"
#include "Buzzer.h"
#include "Led.h"
#include "Seg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*数码管*/
volatile uint8_t  seg_buf[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //BUF:SEG_DISP
/*串口数组*/
uint8_t receiveData[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t transmitData[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*kb1 kb2计数器 1 2*/
volatile uint8_t kb1_counter = 0;
volatile uint8_t kb2_counter = 0;

uint8_t uartFrame[FRAME_SIZE];
volatile uint8_t isFrameReady = 0; 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//外部按键
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == KB1_Pin){
		delay_ms(50);
		if(HAL_GPIO_ReadPin(KB1_GPIO_Port, KB1_Pin) == GPIO_PIN_RESET){
			kb1_counter++;
			seg_buf[1] = kb1_counter/1000 % 10;
			seg_buf[2] = kb1_counter/100 % 10;
			seg_buf[3] = kb1_counter/10 % 10;
			seg_buf[4] = kb1_counter/1 % 10;
			BuildUartFrame();
			//while(HAL_GPIO_ReadPin(KB1_GPIO_Port, KB1_Pin) == GPIO_PIN_RESET);
		}
	}
	if(GPIO_Pin == KB2_Pin){
		delay_ms(50);
		if(HAL_GPIO_ReadPin(KB2_GPIO_Port, KB2_Pin) == GPIO_PIN_RESET){
			kb2_counter++;
			seg_buf[5] = kb2_counter/1000 % 10;
			seg_buf[6] = kb2_counter/100 % 10;
			seg_buf[7] = kb2_counter/10 % 10;
			seg_buf[8] = kb2_counter/1 % 10;
			BuildUartFrame();
			//while(HAL_GPIO_ReadPin(KB2_GPIO_Port, KB2_Pin) == GPIO_PIN_RESET);
		}
	}
}


void BuildUartFrame(void)
{
    // 帧头
    uartFrame[0] = 0xAA;
    uartFrame[1] = 0x55;
    
    // 数据长度(固定8字节)
    uartFrame[2] = 0x08;
    
    // 计数器1数据(4字节)
    uartFrame[3] = (kb1_counter >> 24) & 0xFF;
    uartFrame[4] = (kb1_counter >> 16) & 0xFF;
    uartFrame[5] = (kb1_counter >> 8) & 0xFF;
    uartFrame[6] = kb1_counter & 0xFF;
    
    // 计数器2数据(4字节)
    uartFrame[7] = (kb2_counter >> 24) & 0xFF;
    uartFrame[8] = (kb2_counter >> 16) & 0xFF;
    uartFrame[9] = (kb2_counter >> 8) & 0xFF;
    uartFrame[10] = kb2_counter & 0xFF;
    
    // 计算校验和(帧头之后所有字节的异或)
    uint8_t checksum = 0;
    for(int i = 0; i < 11; i++) {
        checksum ^= uartFrame[i];
    }
    uartFrame[11] = checksum;
    
    // 帧尾
    uartFrame[12] = 0x55;
    uartFrame[13] = 0xAA;
    
    isFrameReady = 1;  // 标记帧已准备好
}

// 发送数据帧
void SendUartFrame(void)
{
    if(isFrameReady)
    {
        // 使用DMA发送数据
        HAL_UART_Transmit_DMA(&huart1, uartFrame, FRAME_SIZE);
        isFrameReady = 0;  // 清除标志
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	Seg_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		for (int i = 1; i <= 8; i++) {
			Seg_disp(seg_buf[i],i);
			delay_ms(1);
		}
		SendUartFrame();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
