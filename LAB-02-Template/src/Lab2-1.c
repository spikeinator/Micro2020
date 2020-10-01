/*
 * Lab2-1.c
 *
 *  Created on: Sep 17, 2020
 *      Author: peckn
 */
/*#include "init.h"
#include "stm32f769xx.h"


#include<stdint.h>


int trigger = 0;
int HALtrigger=0;

int main(){
	Sys_Init();

    //D5 pin C8
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//enable clock
    GPIOC->MODER &= ~GPIO_MODER_MODER8_0;//set input mode
    GPIOC->MODER &= ~GPIO_MODER_MODER8_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;//turn on pull up resistor
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR8_0;

    //D4 pin J0
    GPIO_InitTypeDef pinconfig;
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_0;
    pinconfig.Mode = GPIO_MODE_IT_RISING;
    pinconfig.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOJ,&pinconfig);



    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	asm ("nop");
	asm ("nop");
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
	//SYSCFG->EXTICR &= ~SYSCFG_EXTICR3_EXTI8_2;
	//SYSCFG->EXTICR |= SYSCFG_EXTICR3_EXTI8_1;
	//SYSCFG->EXTICR &= ~SYSCFG_EXTICR3_EXTI8_0;


	// Set interrupt enable for EXTI9_5.
	 NVIC->ISER[23 / 32] = (uint32_t) 1 << (23 % 32);

	// Unmask interrupt.
	 EXTI->IMR |= EXTI_IMR_MR8;


	// Register for rising edge.
	 EXTI->RTSR |= EXTI_RTSR_TR8;

	// And register for the falling edge.
	 EXTI->FTSR &= ~EXTI_FTSR_TR8;


	while(1){
		if(trigger){
			trigger = 0;
			printf("Register Button Pressed\r\n");
		}
		if(HALtrigger){
			HALtrigger=0;
			printf("HAL Button Pressed\r\n");
		}
	}
}

// Non-HAL GPIO/EXTI Handler
void EXTI9_5_IRQHandler() {
	// Clear Interrupt Bit by setting it to 1.
	EXTI->PR |= EXTI_PR_PR8;
	asm ("nop");
	asm ("nop");
	//printf("TEST");

	trigger = 1;

}
void EXTI0_IRQHandler(){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HALtrigger=1;
}*/

