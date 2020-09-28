/*
 * Lab2-4.c
 *
 *  Created on: Sep 21, 2020
 *      Author: peckn
 */

#include "init.h"
#include "stm32f769xx.h"


#include<stdint.h>

void Init_Timer();
void Init_GPIO();
void blinkScreen();


int pb = 0;
int reset = 0;
int of = 0;
float time = 0;
float scores[5];
int blink_wait_time;
int new_wait_time = 1;
int blink = 1;
int trials = 0;
float average;
int i;


int main(){
	Sys_Init();
	Init_Timer();
	Init_GPIO();
	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position


	while(1){
		if(new_wait_time){
			new_wait_time = 0;
			blink_wait_time = (rand() %3) + 1;
			time = 0;
		}

		if(of){
			of = 0;
			time = time + 0.1;
		}

		if((time >= blink_wait_time) && blink){
			blinkScreen();
			time = 0;
			blink = 0;
			pb = 0;
		}

		if(pb && (!blink)){
			pb = 0;
			new_wait_time = 1;
			blink = 1;
			trials++;
			average = 0;
			//sum = 0
			if (trials <= 5){
				scores[trials-1] = time;
				for(i=0; i<5; i++ ){
					average = average+scores[i];
				}

				average = average/trials;
			}

			else{
				for(i=0; i<4; i++){
					scores[i] = scores[i+1];
				}
				scores[4] = time;
				for(i=0; i<5; i++ ){
					average = average+scores[i];
				}
				average = average/5;
			}

			printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
			fflush(stdout);
			printf("Elapsed Time: %.1f\r\n",time);
			printf("Average Time: %.1f\r\n",average);
			/*
			for(i=0;i<5;i++){
				printf("%.1f  ",scores[i] );
				fflush(stdout);
			}
			*/
			//fflush(stdout);
			time = 0;


		}

		if(reset){
			reset = 0;
			time = 0;
			trials =0;
			average = 0;
			new_wait_time = 0;
			blink = 1;
			for(i=0; i<5; i++ ){
				scores[i] = 0;
			}
		}
	}
}


void Init_Timer() {
	// Enable the TIM6 interrupt.
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

void Init_GPIO(){
    //D5 pin C8
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//enable clock
    GPIOC->MODER &= ~GPIO_MODER_MODER8_0;//set input mode
    GPIOC->MODER &= ~GPIO_MODER_MODER8_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;//turn on pull up resistor
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_0;

    //Pushbutton Pin A0
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//enable clock
    GPIOA->MODER &= ~GPIO_MODER_MODER0_0;//set input mode
    GPIOA->MODER &= ~GPIO_MODER_MODER0_1;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0_1;//turn on pull up resistor
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;


    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;//enable sys config clock
	asm ("nop");
	asm ("nop");

	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;


	// Set interrupt enable for EXTI9_5.
	 NVIC->ISER[23 / 32] = (uint32_t) 1 << (23 % 32);
	 EXTI->IMR |= EXTI_IMR_MR8; // Unmask interrupt.
	 EXTI->RTSR |= EXTI_RTSR_TR8;  // Register for rising edge.
	 EXTI->FTSR &= ~EXTI_FTSR_TR8;  // And register for the falling edge.

	 //Set Interrupt enable for EXTI0.
	 NVIC->ISER[6 / 32] = (uint32_t) 1 << (6 % 32);
	 EXTI->IMR |= EXTI_IMR_MR0; // Unmask interrupt.
	 EXTI->RTSR |= EXTI_RTSR_TR0;  // Register for rising edge.
	 EXTI->FTSR &= ~EXTI_FTSR_TR0;  // And register for the falling edge.

}

void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	if(TIM6->SR &= TIM_SR_UIF){
		TIM6->SR &= ~TIM_SR_UIF;

		// Other code here:
		of = 1;
	}

}

void EXTI9_5_IRQHandler() {
	// Clear Interrupt Bit by setting it to 1.
	EXTI->PR |= EXTI_PR_PR8;
	asm ("nop");
	asm ("nop");
	//printf("TEST");

	reset = 1;

}

void EXTI0_IRQHandler() {
	// Clear Interrupt Bit by setting it to 1.
	EXTI->PR |= EXTI_PR_PR0;
	asm ("nop");
	asm ("nop");
	//printf("TEST");

	pb = 1;

}

void blinkScreen(){
	printf("\033[30;47m");
	// Clear and redraw display (flash it & sound the bell).
	printf("\a\033[s\033[2J\033[u");
	fflush(stdout);
	HAL_Delay(1000);

	printf("\033[37;40m");
	// Clear and redraw display (flash it).
	printf("\033[s\033[2J\033[u");
	fflush(stdout);
}
