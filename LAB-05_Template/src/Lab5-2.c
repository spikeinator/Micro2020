/*
 * Lab5-2.c
 *
 *  Created on: Nov 5, 2020
 *      Author: peckn
 */

#include "init.h"

// If needed:
#include <stdio.h>
#include <stdlib.h>

/*
 * For convenience, configure the SPI handler here
 */
SPI_HandleTypeDef spi2;
DMA_HandleTypeDef dma;
DMA_HandleTypeDef dmarx;

void configureDMA(){
	__HAL_RCC_DMA1_CLK_ENABLE();
	dma.Instance = DMA1_Stream4;
	dma.Init.Channel = DMA_CHANNEL_0;
	dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dma.Init.PeriphInc = DMA_PINC_DISABLE;
	dma.Init.MemInc = DMA_MINC_DISABLE;
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma.Init.Mode = DMA_NORMAL;
	dma.Init.Priority = DMA_PRIORITY_LOW;
	dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	//dma.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&dma);

	dmarx.Instance = DMA1_Stream3;
	dmarx.Init.Channel = DMA_CHANNEL_0;
	dmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dmarx.Init.PeriphInc = DMA_PINC_DISABLE;
	dmarx.Init.MemInc = DMA_MINC_DISABLE;
	dmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dmarx.Init.Mode = DMA_NORMAL;
	dmarx.Init.Priority = DMA_PRIORITY_LOW;
	dmarx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	//dmarx.XferCpltCallback = &DMATransferComplete;
	HAL_DMA_Init(&dmarx);


}

void configureSPI()
{
	spi2.Instance = SPI2; // Please use SPI2!
	spi2.Init.Mode = SPI_MODE_MASTER; // Set master mode
	spi2.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	spi2.Init.DataSize = SPI_DATASIZE_8BIT; //Set data to 8 bits
	spi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //Disable CRC
	spi2.Init.Direction = SPI_DIRECTION_2LINES;
	spi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi2.Init.NSS = SPI_NSS_SOFT;
	spi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;

	HAL_SPI_Init(&spi2);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI GPIO initialization structure here
	GPIO_InitTypeDef pinconfig;
	if (hspi->Instance == SPI2)
	{

		//Sets up MOSI on GPIOB Pin 15
		__GPIOB_CLK_ENABLE();
	    pinconfig.Pin = GPIO_PIN_15;
	    pinconfig.Mode = GPIO_MODE_AF_PP;
	    pinconfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    pinconfig.Pull = GPIO_PULLUP;
	    pinconfig.Alternate = GPIO_AF5_SPI2;
	    HAL_GPIO_Init(GPIOB,&pinconfig);

	    //Sets up MISO on GPIOB Pin 14
	    pinconfig.Pin = GPIO_PIN_14;
	    HAL_GPIO_Init(GPIOB,&pinconfig);

	    //__SPI2_CLK_ENABLE();
	    __SPI2_CLK_ENABLE();


	    //Sets up SCK on GPIOA Pin 12
	    __GPIOA_CLK_ENABLE();
	    pinconfig.Pin = GPIO_PIN_12;
	    HAL_GPIO_Init(GPIOA,&pinconfig);

	    //Sets up NSS Pin
	    pinconfig.Pin  = GPIO_PIN_11;
	    pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
	    HAL_GPIO_Init(GPIOA, &pinconfig);

	    __HAL_LINKDMA(hspi,hdmatx,dma);
	    __HAL_LINKDMA(hspi,hdmarx,dmarx);



	}
}

char input[1];
char output[1];
int complete = 0;

int main(void)
{
	Sys_Init();


	// For convenience
	configureSPI();
	configureDMA();

	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout);

	while(1){
		input[0] = 0;

		HAL_UART_Receive(&USB_UART, (uint8_t*) input,1, 10);

		if(input[0] == 0){
			continue;
		}
		else{
			printf("\033[2J\033[;H");
			fflush(stdout);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive_DMA(&spi2, (uint8_t *)input, (uint8_t *)output, 1);
			while(!complete){

			}
			complete = 0;
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
			printf("\033[12B\r\nSPI output: ");
			fflush(stdout);
			HAL_UART_Transmit(&USB_UART, (uint8_t*) output,1, 1000);

			printf("\033[uSPI Input: ");
			fflush(stdout);
			HAL_UART_Transmit(&USB_UART, (uint8_t*) input,1, 1000);
		}
	}
}


void DMA1_Stream3_IRQHandler(){
	complete = 1;
	HAL_DMA_IRQHandler(&dmarx);
}

void DMA1_Stream4_IRQHandler(){
	//complete = 1;
	HAL_DMA_IRQHandler(&dma);
}
