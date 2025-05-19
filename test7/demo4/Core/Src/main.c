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
#include "adc.h"
#include "tim.h"
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

/*延时*/
volatile uint16_t seg_delay_ms = 0; //100ms
volatile uint16_t adc_delay_ms = 0; //100ms
volatile uint16_t led_delay_ms = 0; //100ms
volatile uint16_t led_delay_1000ms = 0; //100ms
volatile uint16_t t_delay_ms = 0; //100ms
//温度
short temp;
float temperture = 15.0;
volatile uint8_t seg_buf[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
volatile uint8_t seg_pos;
uint16_t voltage= 1885;
/*闪烁灯*/
uint8_t led0 = 0;
/*ADC*/
uint16_t value;
float v ;
/*逻辑判断*/
//蜂鸣器 led rgb motor
uint8_t buzzer_on;
uint8_t led1_on;
uint8_t rgb_on;
uint8_t motor_on;

uint8_t led1_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void t_process(){
	temp = DS18B20_Get_Temperature();
	temperture = temp / 10.0;
}



void ADC_process(){
	value = HAL_ADC_GetValue(&hadc1);
	v = (value / 4095.0) *3.3 ;
}

void Seg_process(){
	voltage = (uint16_t)(temperture * 100 );
	// 分离各位数字
	seg_buf[1] = Seg_Table[voltage / 1000 %10];         // 百位(0-3)
  seg_buf[2] = Seg_Table[voltage / 100 %10];         // 百位(0-3)
  seg_buf[3] = Seg_Table[(voltage / 10) % 10];   // 十位
  seg_buf[4] = Seg_Table[voltage % 10];          // 个位
    // 添加小数点(第1位后)
  seg_buf[2] |= 0x80;  // 添加小数点标志(假设您的数码管驱动中最高位控制小数点)
}
void Led_process(){
	if(led1_on == 1){
		led_delay_1000ms++ ;
		if(led_delay_1000ms == 10){
			led1_flag = 1-led1_flag;
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,led1_flag);
			led_delay_1000ms = 0;
		}
	}
	else{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,0);
	}
	
	if(rgb_on == 1){
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,0);
		
	}
	else{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,1);
	}
	
	if(motor_on == 1){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,1);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,0);
	}
	else{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12|GPIO_PIN_13,1);
	}
	
	if(buzzer_on == 1){
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	}
	else{
		HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim1){
		/*
		if(adc_delay_ms++ >= 500){
			adc_delay_ms = 0;
			ADC_process();
		}
		*/
		if(t_delay_ms++ >= 1000){
			t_delay_ms = 0;
			t_process();
		}
		
		if(seg_delay_ms++ >= 100){
			seg_delay_ms = 0;
			Seg_process();
		}
		
		if(led_delay_ms++ >= 100){
			led_delay_ms = 0;
			Led_process();
		}
		seg_pos++;
		if(seg_pos == 9) seg_pos = 1;
		Seg_disp(seg_buf[seg_pos], seg_pos);
		
		//逻辑判断
		if(temperture < 10.0){
			buzzer_on = 1;
			led1_on = 1;
			rgb_on = 1;
			motor_on = 0;
		}
		else if(temperture >25.0){
			buzzer_on = 1;
			led1_on = 1;
			rgb_on = 0 ;
			motor_on = 1;
		}
		else{
			buzzer_on = 0;
			led1_on = 0;
			rgb_on = 0 ;
			motor_on = 0;
			
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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	//开启定时器
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while(DS18B20_Init()){
		HAL_Delay(200);
	}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
