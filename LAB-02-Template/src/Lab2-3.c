/*
 * Lab2-3.c
 *
 *  Created on: Sep 19, 2020
 *      Author: broder
 */
#include "init.h"
#include "stm32f769xx.h"


#include<stdint.h>
int trigger=0;
TIM_HandleTypeDef HALtime7;
float timecount=0;
int main(){
	Sys_Init();
	asm ("nop");
	asm ("nop");
    HALtime7.Instance = TIM7;
    HALtime7.Init.Prescaler = 9999;
    HALtime7.Init.Period = 1080;
    __HAL_RCC_TIM7_CLK_ENABLE();
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    HAL_TIM_Base_Init(&HALtime7);
    HAL_TIM_Base_Start_IT(&HALtime7);


	while(1){
		if (trigger){
			timecount=timecount+0.1;
			printf("%.1f\r\n",timecount);
			trigger=0;
		}
	}
}

void TIM7_IRQHandler(){
	HAL_TIM_IRQHandler(&HALtime7);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM7){
		trigger =1;

	}
}

