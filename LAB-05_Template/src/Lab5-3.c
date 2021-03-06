/*
 * Lab5-3.c
 *
 *  Created on: Nov 9, 2020
 *      Author: broder
 */

#include "init.h"

ADC_HandleTypeDef adc1;
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef dma;
void configureDMA();
void configureADC();
void configureDAC();
void DMATransferComplete();
int complete=0;
uint32_t dma_val;
double adc_val = 0;
double adc_prev1 = 0;
double adc_prev2 = 0;
double y_prev = 0;

double c1 = 0.3125;
double c2 = 0.240385;
double c3 = 0.3125;
double c4 = 0.296875;

double y =0;
double y_c=0;

int main(void){
	Sys_Init();
	configureDMA();
	configureADC();
	configureDAC();
	//HAL_ADC_Start(&adc1);
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	while(1){
		HAL_ADC_Start_DMA(&adc1, &dma_val, (uint32_t)1);
		//while(!complete){

		//}
		adc_val=dma_val;
		//printf("%u\r\n",dma_val);
		/*asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
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
		:[in1] "w" (y_prev),[in2] "w" (c4));*/


		y_c = c1*adc_val + c2*adc_prev1 + c3*adc_prev2 + c4*y_prev;
		//printf("%f\r\n",adc_val);
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adc_val);
		adc_prev2 = adc_prev1;
		adc_prev1 = adc_val;
		y_prev=y_c;
		//HAL_Delay(1);
	}
}

void configureDMA(){
	__HAL_RCC_DMA2_CLK_ENABLE();
	dma.Instance = DMA2_Stream0;
	dma.Init.Channel = DMA_CHANNEL_0;
	dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dma.Init.PeriphInc = DMA_PINC_DISABLE;
	dma.Init.MemInc = DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dma.Init.Mode = DMA_CIRCULAR;
	dma.Init.Priority = DMA_PRIORITY_LOW;
	dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	dma.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&dma);
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
	adc1.Init.DMAContinuousRequests = ENABLE;
	adc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;


	HAL_ADC_Init(&adc1); // Initialize the ADC

	// Configure the ADC channel
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	HAL_ADC_ConfigChannel(&adc1,&sConfig);
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
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac){
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

	    __HAL_LINKDMA(hadc,DMA_Handle,dma);
	}
}
void DMA2_Stream0_IRQHandler(){
	//complete=1;

	HAL_DMA_IRQHandler(&dma);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {

}

void DMATransferComplete(DMA_HandleTypeDef *hdma) {
	complete=1;
}
