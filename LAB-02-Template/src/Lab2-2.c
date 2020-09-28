/*
 * Lab2-2.c
 *
 *  Created on: Sep 17, 2020
 *      Author: peckn
 */
#include "init.h"
#include "stm32f769xx.h"


#include<stdint.h>

void Init_Timer();


int trigger = 0;
float time = 0;

int main(){
	Sys_Init();
	Init_Timer();



	while(1){
		if(trigger){
			trigger = 0;
			time = time + 0.1;
			printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
			fflush(stdout);
			printf("Elapsed Time: %.1f",time);
			fflush(stdout);
		}
	}
}


void Init_Timer() {
	// Enable the TIM6 interrupt.
	// Looks like HAL hid this little gem, this register isn't mentioned in
	//   the STM32F7 ARM Reference Manual....
	NVIC->ISER[54 / 32] = (uint32_t) 1 << (54 % 32);

	// Enable TIM6 clock
	 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	 asm ( "nop" );
	 asm ( "nop" );

	// Set pre-scaler to slow down ticlks
	 TIM6->PSC =4999;

	// Set the Auto-reload Value for 10Hz overflow
	 TIM6->ARR = 2159;

	// Generate update events to auto reload.
	 TIM6->EGR |= TIM_EGR_UG;

	// Enable Update Interrupts.
	 TIM6->DIER |= TIM_DIER_UIE;

	// Start the timer.
	 TIM6->CR1 |= TIM_CR1_ARPE;
	 TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	if(TIM6->SR & TIM_SR_UIF){
		TIM6->SR &= ~TIM_SR_UIF;

	// Other code here:
		trigger = 1;
	}

}
