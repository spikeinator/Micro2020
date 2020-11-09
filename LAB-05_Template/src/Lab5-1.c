/*
 * Lab5-1.c
 *
 *  Created on: Nov 2, 2020
 *      Author: peckn
 */


#include "init.h" // Always need init.h, otherwise nothing will work.
#include<stdint.h>
#include<stdlib.h>

void configureDMA();
void DMATransferComplete();

DMA_HandleTypeDef dma;

char input[1000];
char output[1000];
int cycles;

int main(void)
{
	Sys_Init();
	configureDMA();


	int i;
	for(i=0;i<1000;i++){
		input[i] = i+1;
	}

	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);


	// Enable the DWT_CYCCNT register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout);

	DWT->CYCCNT = 0; // Clear the cycle counter
	for(i=0;i<1000;i++){
		output[i] = input[i];
	}
	cycles = DWT->CYCCNT; // Store the cycle counter
	printf("Copying buffer using C code took %u CPU cycles\r\n",cycles);
	fflush(stdout);

	for(i=0;i<1000;i++){
		output[i] = 0;
	}

	DWT->CYCCNT = 0; // Clear the cycle counter

	HAL_DMA_Start_IT(&dma, (uint32_t)input, (uint32_t)output, 1000);
	while(1){

	}
}


void configureDMA(){

	__HAL_RCC_DMA2_CLK_ENABLE();
	dma.Instance = DMA2_Stream4;
	dma.Init.Channel = DMA_CHANNEL_4;
	dma.Init.Direction = DMA_MEMORY_TO_MEMORY;
	dma.Init.PeriphInc = DMA_PINC_ENABLE;
	dma.Init.MemInc = DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma.Init.Mode = DMA_NORMAL;
	dma.Init.Priority = DMA_PRIORITY_LOW;
	dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	dma.XferCpltCallback = &DMATransferComplete;

	HAL_DMA_Init(&dma);
}

void DMA2_Stream4_IRQHandler(){
	HAL_DMA_IRQHandler(&dma);
}


void DMATransferComplete(DMA_HandleTypeDef *hdma) {
	cycles = DWT->CYCCNT;
	printf("Copying buffer using DMA took %u CPU cycles\r\n",cycles);
	fflush(stdout);
}
