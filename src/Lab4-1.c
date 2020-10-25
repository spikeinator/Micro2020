/*
 * Lab4-1.c
 *
 *  Created on: Oct 20, 2020
 *      Author: peckn
 */


#include "init.h"

ADC_HandleTypeDef adc1;

void configureADC();

int HALtrigger=0;
int rawvalue;
float voltage;
float high_voltage = 0;
float low_voltage = 0;
float average = 0;
float voltage_list [16];
int pushes = 0;
int i;

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureADC();
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_ADC_Start(&adc1);
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	while(1){
		HAL_ADC_PollForConversion(&adc1, HAL_MAX_DELAY);
		rawvalue = HAL_ADC_GetValue(&adc1);

		if(HALtrigger){
			printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
			pushes++;
			average = 0;
			voltage = ((float)rawvalue)*(3.3/4095);

			if (pushes == 1){
				low_voltage = voltage;
				high_voltage = voltage;
			}

			else{
				if(voltage < low_voltage){
					low_voltage = voltage;
				}

				if(voltage > high_voltage){
					high_voltage = voltage;
				}
			}

			if (pushes <= 16){
				voltage_list[pushes-1] = voltage;
				for(i=0; i<16; i++ ){
					average = average + voltage_list[i];
				}

				average = average/pushes;
			}
			else{
				for(i=0; i<15; i++){
					voltage_list[i] = voltage_list[i+1];
				}
				voltage_list[15] = voltage;

				for(i=0; i<16; i++ ){
					average = average + voltage_list[i];
				}

				average = average/16;

			}
			printf("Rawvalue: %x\r\n",rawvalue);
			printf("Voltage: %.6f\r\n",voltage);
			printf("Low Voltage: %.6f\r\n",low_voltage);
			printf("High Voltage: %.6f\r\n",high_voltage);
			printf("Average Voltage: %.6f\r\n", average);

			HALtrigger = 0;
		}
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

	/* Available sampling times:

		ADC_SAMPLETIME_3CYCLES
	  ADC_SAMPLETIME_15CYCLES
		ADC_SAMPLETIME_28CYCLES
		ADC_SAMPLETIME_56CYCLES
		ADC_SAMPLETIME_84CYCLES
		ADC_SAMPLETIME_112CYCLES
		ADC_SAMPLETIME_144CYCLES
		ADC_SAMPLETIME_480CYCLES

	*/

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

    //Configure Blue Pushbutton as an input
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_0;
    //pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Mode = GPIO_MODE_IT_RISING;
    pinconfig.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&pinconfig);


}

void EXTI0_IRQHandler(){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HALtrigger=1;
}

