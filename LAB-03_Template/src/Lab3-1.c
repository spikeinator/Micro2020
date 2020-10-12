/*
 * Lab3-1.c
 *
 *  Created on: Oct 1, 2020
 *      Author: peckn
 */



#include "init.h"

// Main Execution Loop
int main(void) {
	//Initialize the system
	Sys_Init();

	char* esc_msg = "\033[2J\033[;HProgram Halted";
	char* clear_screen = "\033[2J\033[;H";
	char* new_line = "\r\n";

	//HAL_UART_Transmit(&huart6, (uint8_t*) clear_screen, strlen((const char*)clear_screen), 1000);
	HAL_UART_Transmit(&USB_UART, (uint8_t*) clear_screen, strlen((const char*)clear_screen), 1000);
	char input[1];
	while(1){
		input[0] = 0;
		HAL_UART_Receive(&USB_UART, (uint8_t *)input, 1, 10);
		HAL_UART_Receive(&huart6, (uint8_t *)input, 1, 10);


		if (input[0] == 0){
			continue;
		}
		else if(input[0] == 27){
			HAL_UART_Transmit(&USB_UART, (uint8_t*) esc_msg, strlen((const char*)esc_msg), 1000);
			HAL_UART_Transmit(&huart6, (uint8_t*) input, 1, 1000);
			break;
		}
		else{
			HAL_UART_Transmit(&huart6, (uint8_t*) input, 1, 10);
			HAL_UART_Transmit(&USB_UART, (uint8_t*) input, 1, 10);
		}

		HAL_UART_Transmit(&USB_UART, (uint8_t*) new_line, strlen((const char*)new_line), 1000);
	}
}


