#include "DS18B20.h"



/****************************************************************************

函数名：delay_us

功能：微秒级延时

输入：延时数据

输出：无

返回值：无

备注：

****************************************************************************/
void delay_1us(uint16_t us){
	uint16_t differ = 0xffff-us-5;				
	__HAL_TIM_SET_COUNTER(&htim2,differ);	//设定TIM7计数器起始值
	HAL_TIM_Base_Start(&htim2);		//启动定时器	
	
	while(differ < 0xffff-5){	//判断
		differ = __HAL_TIM_GET_COUNTER(&htim2);		//查询计数器的计数值
	}
	HAL_TIM_Base_Stop(&htim2);
}



void DS18B20_IO_IN(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void DS18B20_IO_OUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void DS18B20_Rst(void){
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_1us(750);
	DS18B20_DQ_OUT_HIGH;
	delay_1us(15);
}
	
uint8_t DS18B20_Check(void){
	uint8_t retry = 0;
	DS18B20_IO_IN();
	while(DS18B20_DQ_IN && retry < 200){
		retry++;
		delay_1us(1);
	}
	
	if(retry >= 200)
		return 1;
	else
		retry = 0;
	
	while(!DS18B20_DQ_IN && retry < 240){
		retry++;
		delay_1us(1);
	}
	
	if(retry >= 240)
		return 1;
	
	return 0;
}

uint8_t DS18B20_Read_Bit(void){
	uint8_t data;
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_1us(2);
	DS18B20_DQ_OUT_HIGH;
	DS18B20_IO_IN();
	delay_1us(12);
	
	if(DS18B20_DQ_IN)
		data = 1;
	else
		data = 0;
	
	delay_1us(50);
	return data;
}

uint8_t DS18B20_Read_Byte(void){
	uint8_t i,j,data;
	data = 0;
	for(i=1;i<=8;i++){
		j = DS18B20_Read_Bit();
		data = (j<<7)|(data>>1);
	}
	return data;
}

void DS18B20_Write_Byte(uint8_t data){
	uint8_t j;
	uint8_t testb;
	DS18B20_IO_OUT();
	for(j=1;j<=8;j++){
		testb=data&0x01;
		data=data>>1;
		if(testb){
			DS18B20_DQ_OUT_LOW;
			delay_1us(2);
			DS18B20_DQ_OUT_HIGH;
			delay_1us(60);
		}else{
			DS18B20_DQ_OUT_LOW;
			delay_1us(60);
			DS18B20_DQ_OUT_HIGH;
			delay_1us(2);
		}
	}
}

void DS18B20_Start(void){
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
}

uint8_t DS18B20_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	DS18B20_Rst();
	return DS18B20_Check();
}

short DS18B20_Get_Temperature(void){
	uint8_t temp;
	uint8_t TL,TH;
	short temperature;
	
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0xbe);
	TL = DS18B20_Read_Byte();
	TH = DS18B20_Read_Byte();
	
	if(TH>7){
		TH = ~TH;
		TL = ~TL;
		temp = 0;
	}else
		temp = 1;
	
	temperature = TH;
	temperature <<= 8;
	temperature += TL;
	temperature = (float)temperature*0.625;
	if(temperature)
		return temperature;
	else
		return -temperature;
}
