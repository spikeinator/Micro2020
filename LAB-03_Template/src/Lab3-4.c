//----------------------------------
// Lab 3 - Part 4: Master STM32 - Lab03_spi.c
//----------------------------------
//

#include "init.h"

// If needed:
#include <stdio.h>
#include <stdlib.h>
void GetVersion();
void ChangeID();
void reset();
char input[1];
char output[1];
char UartData[1];
int trigger=0;

/*
 * For convenience, configure the SPI handler here
 */

/*
 * trigger values
 * 0=waiting
 * 1=in esc menu
 * 2=version
 * 3=temperature
 * 4=change ID
 * 5=clear or reset slave terminal
 */
SPI_HandleTypeDef spi2;
void configureSPI()
{
	spi2.Instance = SPI2; // Please use SPI2!
	spi2.Init.Mode = SPI_MODE_MASTER; // Set master mode
	spi2.Init.TIMode = SPI_TIMODE_DISABLE; // Use Motorola mode, not TI mode
	spi2.Init.DataSize = SPI_DATASIZE_8BIT; //Set data to 8 bits
	spi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //Disable CRC
	spi2.Init.Direction = SPI_DIRECTION_2LINES;
	spi2.Init.CLKPhase = SPI_PHASE_2EDGE;
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
	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);

	}
}

int main(void)
{
	Sys_Init();

	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	__HAL_UART_ENABLE_IT(&USB_UART, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
	// For convenience
	configureSPI();


	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	//HAL_SPI_TransmitReceive(&spi2,(uint8_t*)input,(uint8_t*)output, 1,100);
	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);

	//GetVersion();
	ChangeID();
	printf("Hit <ESC> to open the command window");
	fflush(stdout);

	input[0]=0;
	while(1){
		//input[0] = 0;

	//	HAL_UART_Receive(&USB_UART, (uint8_t*) input,1, 10);}
	}
}


void USART1_IRQHandler(){
	HAL_UART_IRQHandler(&USB_UART);
}
void USART6_IRQHandler(){
	HAL_UART_IRQHandler(&huart6);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){
		HAL_UART_Receive_IT(&USB_UART, (uint8_t*)UartData, 1);
		if(trigger==0){
			if(UartData[0]==27){
				trigger=1;
			}
			else{
				//HAL_UART_Transmit_IT(&huart6, (uint8_t*)UartData, 1);

				//need to print back to regular terminal properly formatted
			}
		}
		else if(trigger==1){
			if(UartData[0]==32){
				// get version
				GetVersion();
				trigger=0;
			}
			else if(UartData[0]==33){
				//get temperature
			}
			else if(UartData[0]==34){
				//ChangeID();
			}
				//reset slave terminal

		}
	}
}

void GetVersion(){
	char version[2];
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=0;
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	version[0]=output[0];
	input[0]=1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	version[1]=output[0];
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	printf("\033[2J\033[;H STaTs Version number %x %x\r\n",version[0],version [1]);
	fflush(stdout);
}

void ChangeID(){
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=2;
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);

	input[0]=0x80;
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_Delay(10);

	input[0]=9;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);

	input[0]=12;
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_Delay(10);

	input[0]=9;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_Delay(10);

	printf("\033[2J\033[;H Device ID %x\r\n",output[0]);
	fflush(stdout);
}

void reset(){
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=2;
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);

	input[0]=0x08;
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_Delay(10);
}
