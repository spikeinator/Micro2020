/*
 * Lab4-4.c
 *
 *  Created on: Oct 25, 2020
 *      Author: peckn
 */

#include "init.h"

ADC_HandleTypeDef adc1;

void configureADC();

double adc_val = 0;
double adc_prev1 = 0;
double adc_prev2 = 0;

double c1 = 0.3125;
double c2 = 0.240385;
double c3 = 0.3125;
double c4 = 0.296875;

double y = 0; // Result after filter

int main(void){
	Sys_Init();
	configureADC();

	HAL_ADC_Start(&adc1);
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	while(1){
		HAL_ADC_PollForConversion(&adc1, HAL_MAX_DELAY);
		adc_val = HAL_ADC_GetValue(&adc1);

		asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
		:[out] "+w" (y)
		:[in1] "w" (adc_val),[in2] "w" (c1));


		asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
		:[out] "+w" (y)
		:[in1] "w" (adc_prev1),[in2] "w" (c2));

		asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
		:[out] "+w" (y)
		:[in1] "w" (adc_prev2),[in2] "w" (c3));

		asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
		:[out] "+w" (y)
		:[in1] "w" (adc_prev1),[in2] "w" (c4));


		adc_prev2 = adc_prev1;
		adc_prev1 = adc_val;
	}
}

void configureADC()
{
	ADC_ChannelConfTypeDef sConfig;

	// Enable the ADC Clock.
	__HAL_RCC_ADC1_CLK_ENABLE();

	adc1.Instance = ADC1;
	adc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	adc1.Init.Resolution = ADC_RESOLUTION_12B;
	adc1.Init.ScanConvMode = DISABLE;
	adc1.Init.ContinuousConvMode = ENABLE;
	adc1.Init.DiscontinuousConvMode = DISABLE;
	adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc1.Init.NbrOfConversion = 1;
	adc1.Init.DMAContinuousRequests = DISABLE;
	adc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;


	HAL_ADC_Init(&adc1); // Initialize the ADC

	// Configure the ADC channel
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	HAL_ADC_ConfigChannel(&adc1,&sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init
	GPIO_InitTypeDef pinconfig;
	if (hadc->Instance == ADC1)
	{
		//Sets up ADC1_IN12 on GPIOC Pin 2
		__GPIOC_CLK_ENABLE();
	    pinconfig.Pin = GPIO_PIN_2;
	    pinconfig.Mode = GPIO_MODE_ANALOG;
	    pinconfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    HAL_GPIO_Init(GPIOC,&pinconfig);
	}
}
