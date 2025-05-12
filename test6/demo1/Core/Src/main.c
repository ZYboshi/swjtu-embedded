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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Init_hardware.h"
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
volatile uint8_t seg_pos ;
/*串口数组*/
uint8_t receiveData[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t transmitData[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*闪烁灯*/
uint8_t led0 = 0;
/*时间计时*/
uint16_t time_count = 0 ;
volatile uint8_t hours = 12;    // 初始值12:00:00
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;
volatile uint8_t update_display = 0; // 标志位，用于触发数码管更新

/*延时*/
volatile uint16_t seg_flag = 1;
volatile uint16_t key_flag = 1;
volatile uint16_t delay_1000ms = 1;
/*key*/
//key
volatile uint8_t key_value , key_up , key_down , key_old;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Led_process();
void Seg_process();
void Key_process();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	//定时器3
	if(htim == &htim3){
		/*延时*/
		//1s时间
		delay_1000ms++ ;
		if(delay_1000ms >= 1000){
			delay_1000ms = 0 ;
			Led_process();
		}
		//Seg  10ms
		if(seg_flag++ >= 100){
			seg_flag = 0;
			Seg_process();
		}
		if(key_flag++ >= 100){
			key_flag = 0;
			Key_process();
		}
		
		
		//1ms
		seg_pos++;
		if(seg_pos == 9) seg_pos = 1;
		Seg_disp(seg_buf[seg_pos], seg_pos);

	}
}

void Seg_process(){
	// 更新数码管缓冲区

	seg_buf[1] = Seg_Table[hours / 10];    // 小时的十位
	seg_buf[2] = Seg_Table[hours % 10];    // 小时的个位
	seg_buf[3] = 0x40;                     // 横杠"-"
	seg_buf[4] = Seg_Table[minutes / 10];  // 分钟的十位
	seg_buf[5] = Seg_Table[minutes % 10];  // 分钟的个位
	seg_buf[6] = 0x40;                     // 横杠"-"
	seg_buf[7] = Seg_Table[seconds / 10]; // 秒的十位
	seg_buf[8] = Seg_Table[seconds % 10];  // 秒的个位
	
}

void Key_process(){

	
	//读取
	key_value = key_read();
	key_down	= key_value&(key_value ^ key_old);
	key_up	  = ~key_value&(key_value ^ key_old);
	key_old 	= key_value;
	
	//判断key
	switch(key_value){
		//KB1
		case 1:
			hours++;
			if(hours >= 13) hours = 0;
			break;
		//KB2
		case 2:
			minutes++;
			if(minutes >= 60){hours++; minutes = 0;}
			break;
		//KB3
		case 3:
			seconds++;
			if(seconds >= 60){minutes++ ; seconds = 0;}
			break;
		
	}
}

void Led_process(){
	
			led0 = 1 - led0;
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,led0);
			
      seconds++;
      if (seconds >= 60) {
				seconds = 0;
				minutes++;
        if (minutes >= 60) {
					minutes = 0;
          hours++;
          if (hours > 12) {
						hours = 1; // 12小时制，超过12后回到1
          }
        }
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	/*数码管初始化*/
	seg_buf[1] = Seg_Table[1];
	seg_buf[2] = Seg_Table[2];
	seg_buf[3] = 0x40;
	seg_buf[4] = Seg_Table[0];
	seg_buf[5] = Seg_Table[0];
	seg_buf[6] = 0x40;
	seg_buf[7] = Seg_Table[0];
	seg_buf[8] = Seg_Table[0];
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		
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
