//------------------------------------
// Part 2 of Lab 3 - Interrupt based UART detection
//------------------------------------
//

#include "init.h"
char UartData[1];
int trigger_break=0;

int main(void) {
	//Initialize the system
	Sys_Init();
	printf("\033[2J\033[;H");
	fflush(stdout);
	HAL_NVIC_SetPriority(USART1_IRQn,0x0,0);
	HAL_NVIC_SetPriority(USART6_IRQn,0x1,0);
	printf("before\r\n");
	fflush(stdout);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	printf("after\r\n");
	fflush(stdout);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	__HAL_UART_ENABLE_IT(&USB_UART, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
	char* erase_home = "\033[2J\033[;H";
	char* welcome = "PRESS <ESC> OR <CTRL>+[ TO QUIT\r\n\n";

	UartData[0]=113;
	HAL_UART_Transmit_IT(&USB_UART, (uint8_t*) erase_home, strlen((const char*)erase_home));


	HAL_UART_Transmit_IT(&USB_UART, (uint8_t*) welcome, strlen((const char*)welcome));
	while(1){
		if(trigger_break==1){
			for(int i=0;i<1000000;i++){
				asm("nop");
			}
			break;
		}
	}
}

void USART1_IRQHandler(){
	HAL_UART_IRQHandler(&USB_UART);
	HAL_UART_Receive_IT(&USB_UART, (uint8_t*)UartData, 1);
}
void USART6_IRQHandler(){
	HAL_UART_IRQHandler(&huart6);
	HAL_UART_Receive_IT(&huart6, (uint8_t*)UartData,1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	printf("test2/r/n");
	char* esc_msg = "\033[2J\033[;HProgram Halted";
	//char* new_line = "\r\n";

	if (huart->Instance == USART1){

		HAL_UART_Transmit_IT(&huart6, (uint8_t*)UartData, 1);
	}
	else if (huart->Instance == USART6){

	}

	if(UartData[0] == 27){
		HAL_UART_Transmit_IT(&USB_UART, (uint8_t*)esc_msg, strlen((const char*)esc_msg));
		trigger_break=1;
	}
	else{
		HAL_UART_Transmit_IT(&USB_UART, (uint8_t*)UartData, 1);
		//HAL_UART_Transmit_IT(&USB_UART, (uint8_t*)new_line, strlen((const char*)new_line));
	}
	//HAL_UART_Transmit_IT(&USB_UART, (uint8_t*)new_line, strlen((const char*)new_line));
}
