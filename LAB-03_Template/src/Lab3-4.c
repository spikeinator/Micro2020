//----------------------------------
// Lab 3 - Part 4: Master STM32 - Lab03_spi.c
//----------------------------------
//

#include "init.h"

// If needed:
#include <stdio.h>
#include <stdlib.h>
void GetVersion();
void GetTemp();
void ChangeID();
void reset();
char input[1];
char output[1];
char UartData[1];
char slaveData[1];
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

int commandfunc=0;
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

	GetVersion();
	//GetTemp();
	printf("Hit <ESC> to open the command window");
	fflush(stdout);

	input[0]=0;
	while(1){
		//input[0] = 0;
		if(commandfunc==2){
			GetVersion();
			commandfunc=0;
		}
		else if(commandfunc==3){
			GetTemp();
			commandfunc=0;
		}
		else if(commandfunc==4){
			ChangeID();
			commandfunc=0;
		}
		else if(commandfunc==5){
			reset();
			commandfunc=0;
		}
	//	HAL_UART_Receive(&USB_UART, (uint8_t*) input,1, 10);}
	}
}


void USART1_IRQHandler(){
	HAL_UART_IRQHandler(&USB_UART);
	HAL_UART_Receive_IT(&USB_UART, (uint8_t*)UartData, 1);
}
void USART6_IRQHandler(){
	HAL_UART_IRQHandler(&huart6);
	HAL_UART_Receive_IT(&huart6, (uint8_t*)slaveData, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){
		if(UartData[0]!=27){
			HAL_UART_Transmit_IT(&huart6, (uint8_t*)UartData, 1);
		}

		if(trigger==0){
			if(UartData[0]==27){
				trigger=1;
				printf("\033[2J\033[;H");
				fflush(stdout);
				printf("2 for version\r\n3 for Temp\r\n4 to Change ID\r\n5 to reset Slave Terminal\r\n");
				fflush(stdout);
			}
		}
		else if(trigger==1){

			if(UartData[0]==50){
				printf("\033[2J\033[;H");
				fflush(stdout);
				commandfunc=2;
				trigger=0;
			}
			else if(UartData[0]==51){
				printf("\033[2J\033[;H");
				fflush(stdout);
				commandfunc=3;
				trigger=0;
			}
			else if(UartData[0]==52){
				printf("\033[2J\033[;H");
				fflush(stdout);
				commandfunc=4;
				trigger=0;
			}
			else if(UartData[0]==53){
				printf("\033[2J\033[;H");
				fflush(stdout);
				commandfunc=5;
				trigger=0;
			}
		}
	}
	if (huart->Instance == USART6){
			printf("\033[H\033[20B %c \033[H",slaveData[0]);
			fflush(stdout);
	}
}

void GetVersion(){
	char version[2];
	printf("line1\r\n");
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=0;
	printf("line2\r\n");
	HAL_Delay(100);
	printf("line3\r\n");
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	printf("line4\r\n");
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	printf("line4\r\n");
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	printf("line5\r\n");
	version[0]=output[0];
	input[0]=9;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	version[1]=output[0];
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	printf("\033[2J\033[;H STaTs Version number %x.%x\r\n",version[0],version [1]);
	fflush(stdout);
}
void GetTemp(){

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=2;
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	input[0]=0x02;
	HAL_SPI_Transmit(&spi2, (uint8_t*)input,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	int tempReady=0;
	while(tempReady==0){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
		input[0]=3;
		HAL_Delay(1000);
		HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
		HAL_Delay(10);
		HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
		if((output[0] & 0x09)== 0x09){
			tempReady=1;
		}
		printf("%x\r\n", output[0]);
		fflush(stdout);
	}
	char Temps[2];
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=5;
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	Temps[0]=output[0];

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	input[0]=6;
	HAL_Delay(1000);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	Temps[1]=output[0];
	int tempfinal=(Temps[1]<<8|Temps[0]);
	printf("The temperature in decimal is %d\r\n", tempfinal);
	fflush(stdout);
}

void ChangeID(){
	input[0]=9;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_SPI_Transmit(&spi2, (uint8_t *)input, 1,100);
	HAL_Delay(10);
	HAL_SPI_Receive(&spi2, (uint8_t*)output,1 ,100);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_Delay(10);

	printf("\033[2J\033[;H Old Device ID %x\r\n",output[0]);
	fflush(stdout);

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

	printf("New Device ID %x\r\n",output[0]);
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


