/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <includes.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* UCOSIII���������ȼ��û�������ʹ��:
 * ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 * ���ȼ�0���жϷ������������� OS_IntQTask()
 * ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 * ���ȼ�2����ʱ���� OS_TmrTask()
 * ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 * ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
 */

/* �������ȼ� */
#define START_TASK_PRIO		3
#define LED0_TASK_PRIO		4
#define LED1_TASK_PRIO		5
#define PRINT_TASK_PRIO    6
#define LED_TASK_PRIO     7
#define Buzzer_TASK_PRIO     8
#define Motor_TASK_PRIO     8

/* �����ջ��С	*/
#define START_STK_SIZE 		512
#define LED0_STK_SIZE 		128
#define LED1_STK_SIZE 		128
#define PRINT_STK_SIZE      128
#define LED_STK_SIZE      128
#define Buzzer_STK_SIZE      128
#define Motor_STK_SIZE      128


/* ������ƿ� */
OS_TCB StartTaskTCB;
OS_TCB Led0TaskTCB;
OS_TCB Led1TaskTCB;
OS_TCB PrintTaskTCB;
OS_TCB LedplayTCB;
OS_TCB BuzzerTCB;
OS_TCB MotorTCB;

/* �����ջ */	
CPU_STK START_TASK_STK[START_STK_SIZE];
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
CPU_STK PRINT_TASK_STK[PRINT_STK_SIZE];
CPU_STK LED_TASK_STK[LED_STK_SIZE];
CPU_STK Buzzer_TASK_STK[LED_STK_SIZE];
CPU_STK Motor_TASK_STK[LED_STK_SIZE];


/* ���������� */
void start_task(void *p_arg);
void led0_task(void *p_arg);
void led1_task(void *p_arg);
void print_task(void *p_arg);
void led_play(void *p_arg);
void Buzzer(void *p_arg);
void Motor(void *p_arg);


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    OS_ERR err;
	CPU_SR_ALLOC();
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	OSInit(&err);		//��ʼ��UCOSIII
    OS_CRITICAL_ENTER();//�����ٽ���
    //������ʼ����
    OSTaskCreate((OS_TCB 	* )&StartTaskTCB,			//������ƿ�
                 (CPU_CHAR* )"start task", 				//��������
                 (OS_TASK_PTR)start_task, 				//������
                 (void		* )0,						//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY)0,						//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,						//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,						//�û�����Ĵ洢��
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
    OS_CRITICAL_EXIT();	//�˳��ٽ���	

    OSStart(&err); //����������ϵͳ������Ȩ����uC/OS-III

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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

  /** Initializes the CPU, AHB and APB busses clocks
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
  /** Initializes the CPU, AHB and APB busses clocks
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
//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	BSP_Init();                                                   /* Initialize BSP functions */
    //CPU_Init();
    //Mem_Init();                                                 /* Initialize Memory Management Module */

#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  		//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN			//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  		//��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	/* ����LED0���� */
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	/* ����LED1���� */
	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	
		/* ����LED��ʾ���� */
	OSTaskCreate((OS_TCB 	* )&LedplayTCB,		
				 (CPU_CHAR	* )"led play", 		
                 (OS_TASK_PTR )led_play, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     	
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		
/* ��������������� */
	OSTaskCreate((OS_TCB 	* )&PrintTaskTCB,		
				 (CPU_CHAR	* )"print task", 		
                 (OS_TASK_PTR )print_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )PRINT_TASK_PRIO,     	
                 (CPU_STK   * )&PRINT_TASK_STK[0],	
                 (CPU_STK_SIZE)PRINT_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRINT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		
								 
/* ����������� */
	OSTaskCreate((OS_TCB 	* )&MotorTCB,		
				 (CPU_CHAR	* )"Motor", 		
                 (OS_TASK_PTR )Motor, 			
                 (void		* )0,					
                 (OS_PRIO	  )Motor_TASK_PRIO,     	
                 (CPU_STK   * )&Motor_TASK_STK[0],	
                 (CPU_STK_SIZE)Motor_STK_SIZE/10,	
                 (CPU_STK_SIZE)Motor_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);									 
								 
		/* �������������� */
	OSTaskCreate((OS_TCB 	* )&BuzzerTCB,		
				 (CPU_CHAR	* )"Buzzer", 		
                 (OS_TASK_PTR )Buzzer, 			
                 (void		* )0,					
                 (OS_PRIO	  )Buzzer_TASK_PRIO,     	
                 (CPU_STK   * )&Buzzer_TASK_STK[0],	
                 (CPU_STK_SIZE)Buzzer_STK_SIZE/10,	
                 (CPU_STK_SIZE)Buzzer_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
					 
								 
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}

/* led0������ */
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		OSTimeDly(500, OS_OPT_TIME_DLY, &err);
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		OSTimeDly(500, OS_OPT_TIME_DLY, &err);
	}
}

/* led1������ */
void led1_task(void *p_arg)
{
  OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		OSTimeDly(2000, OS_OPT_TIME_DLY, &err);	
	}
}

/* ��������������� */
void print_task(void *p_arg)
{
	uint32_t num = 0;
	OS_ERR err;
	p_arg = p_arg;
	CPU_SR_ALLOC();
	
	while(1)
	{
		num++;
		//OS_CRITICAL_ENTER();	//�����ٽ���
		printf("num = %d\r\n",num);
		//OS_CRITICAL_EXIT();		//�˳��ٽ���
		OSTimeDly(500, OS_OPT_TIME_DLY, &err);
	}
}

/* ������������ */
void led_play(void *p_arg)
{
	unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
	unsigned char s[]={2,0,2,2,6,4,8,1};
	unsigned char i,n;	
	OS_ERR err;
	p_arg = p_arg;
	CPU_SR_ALLOC();
	
	while(1)
	{			
		for(n=8;n<16;n++)
		{
					 for(i=0;i<8;i++)
					{
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,(table[s[15-n]]>>(7-i))&0x01);
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
					}
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,1);
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,0);
			HAL_GPIO_WritePin(GPIOF,0x01<<n,0);
			OSTimeDly(1, OS_OPT_TIME_DLY, &err);
			HAL_GPIO_WritePin(GPIOF,0x01<<n,1);
		}
	}
}


/* ���������� */

void Buzzer(void *p_arg)
{
	unsigned int SONG_tone[]={441,441,495,441,589,556,441,441,495,441,661,589,441,441,882,742,589,556,495,786,786,742,589,661,589,0};
  unsigned int SONG_long[]={9,3,12,12,12,24,9,3,12,12,12,24,9,3,12,12,12,12,12,9,3,12,12,12,24,0};
	unsigned int i,j,k;
	OS_ERR err;
	p_arg = p_arg;
	CPU_SR_ALLOC();
	while(1)
	{		
    for(i=0;i<25;i++)
		{
			uint32_t period_us = 1440000 / SONG_tone[i]; 
			uint32_t half_period_us = period_us / 2;
			uint32_t cycles = SONG_long[i] * 100000 / period_us; 
	 
			for (j = 0; j < cycles; j++)
			{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); 
					k=0;while(k<5*half_period_us)k++;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);  
					k=0;while(k<5*half_period_us)k++;
			}
		}	
	}	
}

/* ���ת�� */
void Motor(void *p_arg)
{
		unsigned int i,j,k;
		OS_ERR err;
		p_arg = p_arg;
		CPU_SR_ALLOC();
		while(1)
		{			
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,1);
				OSTimeDly(1, OS_OPT_TIME_DLY, &err);										
		}	

}


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
