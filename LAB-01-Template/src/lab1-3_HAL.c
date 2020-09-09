/*
 * lab1-3_HAL.c
 *
 *  Created on: Sep 8, 2020
 *      Author: peckn
 */

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

    //void HAL_GPIO_Init(GPIOJ,&pinconfig)

    GPIO_InitTypeDef pinconfig;

    //Sets up LD1 on GPIOJ Pin 13
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_13;
    pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
    //pinconfig.Pull = GPIO_PULLUP;
    pinconfig.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ,&pinconfig);

    //Sets up LD2 on GPIOJ Pin 5
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_5;
    pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
    //pinconfig.Pull = GPIO_PULLUP;
    pinconfig.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ,&pinconfig);

    //Sets up LD3 on GPIOA Pin 12
    __HAL_RCC_GPIOA_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_12;
    pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
    //pinconfig.Pull = GPIO_PULLUP;
    pinconfig.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOA,&pinconfig);

    //Sets up LD4 on GPIOD Pin 4
    __HAL_RCC_GPIOD_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_4;
    pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
    //pinconfig.Pull = GPIO_PULLUP;
    pinconfig.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD,&pinconfig);

    //Turn on all 4 LEDS to make sure configured correctly
    HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_13,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);

    //Sets up D0 input on GPIOC Pin 7
    __HAL_RCC_GPIOC_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_7;
    pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC,&pinconfig);

    //Sets up D1 input on GPIOC Pin 6
    __HAL_RCC_GPIOC_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_6;
    pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC,&pinconfig);

    //Sets up D2 input on GPIOJ Pin 1
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_1;
    pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOJ,&pinconfig);

    //Sets up D3 input on GPIOF Pin 6
    __HAL_RCC_GPIOF_CLK_ENABLE();
    pinconfig.Pin = GPIO_PIN_6;
    pinconfig.Mode = GPIO_MODE_INPUT;
    pinconfig.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOF,&pinconfig);


    while(1)
    {
    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)){  //Turn on LED1 if SW0 is on
    	    HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_13,GPIO_PIN_SET);
    	}

    	else{ //Turn off LED1 if SW0 is off
    	    HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_13,GPIO_PIN_RESET);
    	}

    	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)){ //Turn on LED2 if SW1 is on
    	    HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_SET);
    	}

    	else{ //Turn off LED2 if SW1 is off
    		HAL_GPIO_WritePin(GPIOJ,GPIO_PIN_5,GPIO_PIN_RESET);
    	}

    	if(HAL_GPIO_ReadPin(GPIOJ,GPIO_PIN_1)){  //Turn on LED3 if SW2 is on
    		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
    	}

    	else{ //Turn off LED3 if SW2 is off
    		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
    	}

    	if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6)){ //Turn on LED4 if SW 3 is on. NOTE:  This LED uses inverted Logic
    		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
    	}

    	else{ //Turn off LED4 if SW 3 is off. NOTE:  This LED uses inverted Logic
    		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
    	}
	}
}
