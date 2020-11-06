/*
 * Lab4-1.c
 *
 *  Created on: Oct 20, 2020
 *      Author: peckn
 */


#include "init.h"

ADC_HandleTypeDef adc1;
DAC_HandleTypeDef hdac;
void configureDAC();
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
	configureDAC();
	//HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	while(1){
		for(int output=0; output<4096; output++){
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, output);
			HAL_Delay(1);
		}
	}
}
void configureDAC(){
	__HAL_RCC_DAC_CLK_ENABLE();

	DAC_ChannelConfTypeDef dacConfig;

	hdac.Instance=DAC;
	HAL_DAC_Init(&hdac);
	dacConfig.DAC_Trigger=DAC_TRIGGER_NONE;
	dacConfig.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;
	HAL_DAC_ConfigChannel(&hdac, &dacConfig, DAC_CHANNEL_1);

}
/*void configureADC()
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
	/*sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	HAL_ADC_ConfigChannel(&adc1,&sConfig);
}*/


void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{

// GPIO init
	GPIO_InitTypeDef pinconfig;
	if (hdac->Instance == DAC1)
	{
		//Sets up DAC on GPIOA Pin 4
		__GPIOA_CLK_ENABLE();
	    pinconfig.Pin = GPIO_PIN_4;
	    pinconfig.Mode = GPIO_MODE_ANALOG;
	    pinconfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    HAL_GPIO_Init(GPIOA,&pinconfig);
	}

    //Configure Blue Pushbutton as an input
    /*__HAL_RCC_GPIOA_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_0;
    //pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Mode = GPIO_MODE_IT_RISING;
    pinconfig.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&pinconfig);*/


}

void EXTI0_IRQHandler(){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HALtrigger=1;
}

