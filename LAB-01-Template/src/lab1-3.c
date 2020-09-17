
//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>


//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init(); // This always goes at the top of main (defined in init.c)

    //D0 pin C7
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//enable clock
    GPIOC->MODER &= ~GPIO_MODER_MODER7_0;//set input mode
    GPIOC->MODER &= ~GPIO_MODER_MODER7_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR7_1;//turn on pull up resistor
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR7_0;

    //D1 pin C6
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//enable clock
    GPIOC->MODER &= ~GPIO_MODER_MODER6_0;//set input mode
    GPIOC->MODER &= ~GPIO_MODER_MODER6_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;//turn on pull up resistor
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR6_0;

    //D2 pin J1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;//enable clock
    GPIOJ->MODER &= ~GPIO_MODER_MODER1_0;//set input mode
    GPIOJ->MODER &= ~GPIO_MODER_MODER1_1;
    GPIOJ->PUPDR &= ~GPIO_PUPDR_PUPDR1_1;//turn on pull up resistor
    GPIOJ->PUPDR |= GPIO_PUPDR_PUPDR1_0;

    //D3 pin F6
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;//enable clock
    GPIOF->MODER &= ~GPIO_MODER_MODER6_0;//set input mode
    GPIOF->MODER &= ~GPIO_MODER_MODER6_1;
    GPIOF->PUPDR &= ~GPIO_PUPDR_PUPDR6_1;//turn on pull up resistor
    GPIOF->PUPDR |= GPIO_PUPDR_PUPDR6_0;

    //L1 pin J13
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;//Enable Clock
    GPIOJ->MODER &= ~GPIO_MODER_MODER13_1;//set output mode
    GPIOJ->MODER |= GPIO_MODER_MODER13_0;
    GPIOJ->OTYPER &= ~GPIO_OTYPER_OT_13;//configure output type
    GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1; //set output speed
    GPIOJ->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13_0;

    //L2 pin J5
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;//Enable Clock
    GPIOJ->MODER &= ~GPIO_MODER_MODER5_1;//set output mode
    GPIOJ->MODER |= GPIO_MODER_MODER5_0;
    GPIOJ->OTYPER &= ~GPIO_OTYPER_OT_5;//configure output type
    GPIOJ->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1; //set output speed
    GPIOJ->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5_0;

    //L3 pin A12
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//Enable Clock
    GPIOA->MODER &= ~GPIO_MODER_MODER12_1;//set output mode
    GPIOA->MODER |= GPIO_MODER_MODER12_0;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_12;//configure output type
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1; //set output speed
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR12_0;

    //L4 pin D4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;//Enable Clock
    GPIOD->MODER &= ~GPIO_MODER_MODER4_1;//set output mode
    GPIOD->MODER |= GPIO_MODER_MODER4_0;
    GPIOD->OTYPER &= ~GPIO_OTYPER_OT_4;//configure output type~
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1; //set output speed
    GPIOD->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4_0;

    GPIOJ->ODR |= GPIO_ODR_ODR_13;
    GPIOJ->ODR |= GPIO_ODR_ODR_5;
    GPIOA->ODR |= GPIO_ODR_ODR_12;
    GPIOD->ODR &= ~GPIO_ODR_ODR_4;





    while(1)
    {
    	if(!(GPIOC->IDR & GPIO_IDR_IDR_7)){
    		GPIOJ->ODR |= GPIO_ODR_ODR_13;
    	}
    	else{
    		GPIOJ->ODR &= ~GPIO_ODR_ODR_13;
    	}

    	if(!(GPIOC->IDR & GPIO_IDR_IDR_6)){
    		GPIOJ->ODR |= GPIO_ODR_ODR_5;
    	}
    	else{
    		GPIOJ->ODR &= ~GPIO_ODR_ODR_5;
    	}

    	if(!(GPIOJ->IDR & GPIO_IDR_IDR_1)){
    		GPIOA->ODR |= GPIO_ODR_ODR_12;
    	}
    	else{
    		GPIOA->ODR &= ~GPIO_ODR_ODR_12;
    	}

    	if(!(GPIOF->IDR & GPIO_IDR_IDR_6)){
    		GPIOD->ODR &= ~GPIO_ODR_ODR_4;
    	}
    	else{
    		GPIOD->ODR |= GPIO_ODR_ODR_4;
    	}
	}
}

