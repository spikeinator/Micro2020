/*
 * template.c
 *
 *  Created on: Dec 9, 2020
 *      Author: broder
 */





/*
 * in order to use this, the lab sid gave us works but some things need to be re-added to the build
 * -Libraries/BSP/Components/ft6x06
 * -Libraries/BSP/stm32xx.../the ts.c one
 *
 * I tried formatting the final template but was getting all kinds of error messaging so I just cleaned up this one,
 * worst comes to worse we just try and use this or copy it over
 *
 *
 * */
#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
#include "stdio.h"
#include "stdlib.h"


#include "ff_gen_drv.h"
#include "sd_diskio.h"

#include "init.h"

/* Defines */
#define LCD_FRAME_BUFFER        0xC0000000
#define JPEG_OUTPUT_DATA_BUFFER 0xC0200000

/* Global Variables */
void draw(int frame);//Main LCD updating function

//various char arrays that are printed to the screen
char totalstr[12]="Total today";
char maxstr[9]="Max today";
char currentstr[22]="Currently in the room";
char prntchar[3];
char backstr[13]="Back to main";
char exitstr[5]="EXIT";
char recent[13]="Recent times";

//variables to track which frame is currently on display
int leftframe=0;
int rightframe=2;
int currentframe=1;
int nextframe=1;

//Numbers to track
int currentint=0;//current amount of people in the room
int maxint=0;//Maximum amount of people in the room at once
int totalint=0;//How many people have been in the room today

//the bounds used to determine where the buttons are
uint16_t xbound1[2]={0,0};
uint16_t xbound2[2]={0,0};
uint16_t xbound3[2]={0,0};
uint16_t ybound1[2]={0,0};
uint16_t ybound2[2]={0,0};
uint16_t ybound3[2]={0,0};



//-------------------------------------
TIM_HandleTypeDef HALtime7;
int trigger1 = 0;
int trigger2 = 0;
int trigger_time = 0;

uint32_t HCSR04_Read1();
uint32_t HCSR04_Read2();
uint32_t local_time1, sensor_time1;

uint32_t local_time2, sensor_time2;
uint32_t check_distance = 1000;

double time_running = 0;
int minutes = 0;
int hours = 0;
int seconds = 0;

uint32_t distance1;
uint32_t distance2;

int check_sensors = 1;
int sensor_clear = 1;
int check2 = 1;
int people = 0;
int entrance_times[5][3];
int num_entrance = 0;
int num_leave = 0;
int leave_times[5][3];
int i;

int read_delay;
int clear_delay;
//--------------------------------------------

//Text specific variables
Text_AlignModeTypdef mode;//it bothers me a lot that this is a Typdef and not a Typedef


int main(void)
{
	Sys_Init();

	asm ("nop");
	asm ("nop");

	//Set up 1us Timer
    HALtime7.Instance = TIM7;
    HALtime7.Init.Prescaler = 11;
    HALtime7.Init.Period = 8;
    __HAL_RCC_TIM7_CLK_ENABLE();
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    HAL_TIM_Base_Init(&HALtime7);
    HAL_TIM_Base_Start_IT(&HALtime7);

	GPIO_InitTypeDef pinconfig;

	//Sensor 1 Trig D7
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	pinconfig.Pin = GPIO_PIN_3;
	pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
	pinconfig.Pull = GPIO_NOPULL;
	pinconfig.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOJ, &pinconfig);

	//Sensor 1 Echo D6
	__HAL_RCC_GPIOF_CLK_ENABLE();
	pinconfig.Pin = GPIO_PIN_7;
	pinconfig.Mode = GPIO_MODE_INPUT;
	pinconfig.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF,&pinconfig);

	//Sensor 2 Trig D5
	__HAL_RCC_GPIOC_CLK_ENABLE();
	pinconfig.Pin = GPIO_PIN_8;
	pinconfig.Mode = GPIO_MODE_OUTPUT_PP;
	pinconfig.Pull = GPIO_NOPULL;
	pinconfig.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &pinconfig);

	//Sensor 2 Echo D4
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	pinconfig.Pin = GPIO_PIN_0;
	pinconfig.Mode = GPIO_MODE_INPUT;
	pinconfig.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOJ,&pinconfig);

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    printf("\033c"); // Reset device
    fflush(stdout);


    TS_StateTypeDef screen; //create a touchscreen variable to be used for intializations and getting updates

    //initiate the LCD and select which layer to draw on
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);
    //draw Main page to start
    draw(1);
    //Initialize touchscreen and if necessary check to make sure it worked properly (check should be equal to 0 if it is)
    uint8_t check = BSP_TS_Init((uint16_t) 800, (uint16_t) 480);


    //main while loop
    while(1){
        /*put your ultrasonic functions here
    	look at variables currentint, maxint, and totalint when tracking people. they need to be changed to 0, was just using those numbers for testing
    	P.S. the function    draw(currentframe);      will refresh whatever page is currently drawn (use after updating occupancy ints)
    	*/
    	if(check_sensors && sensor_clear){
    		sensor_time1 = HCSR04_Read1();
    		sensor_time2 = HCSR04_Read2();
    		distance1  = sensor_time1 * .034/2;
    		distance2  = sensor_time2 * .034/2;
    		printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    		fflush(stdout);
    		printf("distance1: %ld\r\n",sensor_time1);
    		printf("distance2: %ld\r\n",sensor_time2);
    		read_delay = 0;
    		check_sensors = 0;
    	}


 	   //HAL_Delay(200);

 	   //Check If Someone steps in front of Sensor 1
 	   if((sensor_time1 < check_distance) && sensor_clear){
 		   //Record time and add to list of last 5 entrance times
 		   if(num_entrance < 5){
 			   entrance_times[num_entrance][0] = hours;
 			   entrance_times[num_entrance][1] = minutes;
 			   entrance_times[num_entrance][2] = seconds;
 			   num_entrance++;
 		   }
 		   else{
 			   for(i=0;i<4;i++){
 				   entrance_times[i][0] = entrance_times[i+1][0];
 				   entrance_times[i][1] = entrance_times[i+1][1];
 				   entrance_times[i][2] = entrance_times[i+1][2];
 			   }
 			   entrance_times[4][0] = hours;
 			   entrance_times[4][1] = minutes;
 			   entrance_times[4][2] = seconds;
 		   }

 		   currentint++; //Increment Number of people in the room
 		   totalint++;
 		   draw(currentframe);
 		   clear_delay = 0;
 		   sensor_clear = 0;

 		   if(currentint > maxint){
 			   maxint = currentint;
 			   draw(currentframe);
 		   }

 		   //HAL_Delay(2000); //Wait for 2 seconds so person can move
 	   }

 	   if((sensor_time2 < check_distance) && sensor_clear){
 		   //Record time and add to list of last 5 entrance times
 		   if(num_leave < 5){
 			   leave_times[num_leave][0] = hours;
 			   leave_times[num_leave][1] = minutes;
 			   leave_times[num_leave][2] = seconds;

 			   num_leave++;
 		   }
 		   else{
 			   for(i=0;i<4;i++){
 				   leave_times[i][0] = leave_times[i+1][0];
 				   leave_times[i][1] = leave_times[i+1][1];
 				   leave_times[i][2] = leave_times[i+1][2];
 			   }
 			   leave_times[4][0] = hours;
 			   leave_times[4][1] = minutes;
 			   leave_times[4][2] = seconds;
 		   }
 		   if(currentint == 0){
 			   currentint = 0;
 		   }
 		   else{
 			  currentint -= 1; //Decrement Number of people in the room
 		   }

 		   draw(currentframe);
 		   clear_delay = 0;
 		   sensor_clear = 0;
 		   //HAL_Delay(2000); //Wait for 2 seconds so person can move
 	   }





    	//update touchscreen typedef to check for any recent inputs
    	check = BSP_TS_GetState(&screen);
        if(screen.touchDetected!=0){// checks if screen has been touched

        	//check various bounds for on-screen buttons
        	if(((screen.touchX[0]>0)&&(screen.touchX[0]<100))&&((screen.touchY[0]>0)&&(screen.touchY[0]<80))){//did the user hit the exit button
        		draw(10);
        		break;
        	}
        	else if(((screen.touchX[0]>xbound1[0])&&(screen.touchX[0]<xbound1[1]))&&((screen.touchY[0]>ybound1[0])&&(screen.touchY[0]<ybound1[1]))){//did the user hit the left button
        		draw(leftframe);
        	}
        	else if(((screen.touchX[0]>xbound2[0])&&(screen.touchX[0]<xbound2[1]))&&((screen.touchY[0]>ybound2[0])&&(screen.touchY[0]<ybound2[1]))){//did the user hit the right button
        		draw(rightframe);

        	}
        	else if(((screen.touchX[0]>xbound3[0])&&(screen.touchX[0]<xbound3[1]))&&((screen.touchY[0]>ybound3[0])&&(screen.touchY[0]<ybound3[1]))){//did the user hit the middle button
        		draw(nextframe);

        	}



        	//reset variables
        	screen.touchDetected=0;
    		screen.touchX[0]=0;
    		screen.touchY[0]=0;


        }
    }

}



//the main LCD drawing function, slightly repetitive but easier to understand when done this way.
void draw(int frame){
    //clear the screen
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	char hr[3];
	char min[3];
	char sec[3];
	uint16_t timey=0;

	//Draw the Exit button
    BSP_LCD_SetTextColor(LCD_COLOR_RED);//Text color = fill color for buttons
    BSP_LCD_FillRect(0, 0, 100, 80);//draw the button
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);//reformat text
    BSP_LCD_DisplayStringAt(14,28, (uint8_t *)exitstr, mode);// label button

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

	itoa(hours,hr,10);
	itoa(minutes,min,10);
	itoa(seconds,sec,10);
    BSP_LCD_DisplayStringAt(640,timey, (uint8_t *)hr, mode);
    BSP_LCD_DisplayChar(680, timey, 58);
    BSP_LCD_DisplayStringAt(700,timey, (uint8_t *)min, mode);
    BSP_LCD_DisplayChar(740, timey, 58);
    BSP_LCD_DisplayStringAt(760,timey, (uint8_t *)sec, mode);
    timey=136;


    if(frame==0){//draws the "total today" screen

    	//the three common frams (0,1 & 2) follow the same layout with minor differences in where buttons are placed and what data is displayed
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);//to ensure shapes are black
        BSP_LCD_FillRect(580, 320, 220, 160);//draw the button
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);//reformat text
        BSP_LCD_DisplayStringAt(590,388, (uint8_t *)backstr, mode);//label
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);//reformat text
        BSP_LCD_DisplayStringAt(329,136, (uint8_t *)totalstr, mode);//Label data
        itoa(totalint,prntchar,10);//Data is stored as an int and must be converted to char array to display
        BSP_LCD_DisplayStringAt(380,160, (uint8_t *)prntchar, mode);//display data

        //change the bounds in order to show that there is only one button that is pressable
        xbound1[0]=0;
        xbound1[1]=0;
        ybound1[0]=0;
        ybound1[1]=0;

        xbound2[0]=580;
        xbound2[1]=800;
        ybound2[0]=320;
        ybound2[1]=480;

        xbound3[0]=0;
        xbound3[1]=0;
        ybound3[0]=0;
        ybound3[1]=0;

        //track what frame you are on and what possible frames are next
        leftframe=5; //there is no frame 3 so nothing will change
        rightframe=1;
        currentframe=0;



    }
    else if(frame==1){//draws the "currently in the room" frame

    	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    	BSP_LCD_FillRect(0, 320, 220, 160);
        BSP_LCD_FillRect(580, 320, 220, 160);
        BSP_LCD_FillRect(290, 320, 220, 160);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(2,388, (uint8_t *)totalstr, mode);
        BSP_LCD_DisplayStringAt(630,388, (uint8_t *)maxstr, mode);
        BSP_LCD_DisplayStringAt(300,388, (uint8_t *)recent, mode);

        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(211,136, (uint8_t *)currentstr, mode);
        itoa(currentint,prntchar,10);
        BSP_LCD_DisplayStringAt(380,160, (uint8_t *)prntchar, mode);
        //change the bounds to ensure two buttons are pressable
        xbound1[0]=0;
        xbound1[1]=220;
        ybound1[0]=320;
        ybound1[1]=480;
        xbound2[0]=580;
        xbound2[1]=800;
        ybound2[0]=320;
        ybound2[1]=480;
        xbound3[0]=290;
        xbound3[1]=510;
        ybound3[0]=320;
        ybound3[1]=480;

        leftframe=0;
        rightframe=2;
        currentframe=1;
        nextframe=3;
    }
    else if (frame ==2){//draws the "total of the day"  frame

        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(0, 320, 220, 160);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0,388, (uint8_t *)backstr, mode);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(329,136, (uint8_t *)maxstr, mode);
        itoa(maxint,prntchar,10);
        BSP_LCD_DisplayStringAt(380,160, (uint8_t *)prntchar, mode);

        //change the bounds so only the left button is pressable.
        xbound1[0]=0;
        xbound1[1]=220;
        ybound1[0]=320;
        ybound1[1]=480;

        xbound2[0]=0;
        xbound2[1]=0;
        ybound2[0]=0;
        ybound2[1]=0;

        xbound3[0]=0;
        xbound3[1]=0;
        ybound3[0]=0;
        ybound3[1]=0;

        leftframe=1;
        rightframe=5;
        currentframe=2;
    }
    else if (frame==3){

    	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(290, 320, 220, 160);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(300,388, (uint8_t *)backstr, mode);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(300,50, (uint8_t *)recent, mode);
        char entr[8]="Entered";
        char lft[5]="Left";
        BSP_LCD_DisplayStringAt(10,timey, (uint8_t *)entr, mode);
        BSP_LCD_DisplayStringAt(700,timey, (uint8_t *)lft, mode);
        timey=timey+30;


        for(i=0;i<5;i++){
        	itoa(entrance_times[i][0],hr,10);
        	itoa(entrance_times[i][1],min,10);
        	itoa(entrance_times[i][2],sec,10);
            BSP_LCD_DisplayStringAt(10,timey, (uint8_t *)hr, mode);
            BSP_LCD_DisplayChar(50, timey, 58);
            BSP_LCD_DisplayStringAt(70,timey, (uint8_t *)min, mode);
            BSP_LCD_DisplayChar(110, timey, 58);
            BSP_LCD_DisplayStringAt(130,timey, (uint8_t *)sec, mode);

        	itoa(leave_times[i][0],hr,10);
        	itoa(leave_times[i][1],min,10);
        	itoa(leave_times[i][2],sec,10);
            BSP_LCD_DisplayStringAt(640,timey, (uint8_t *)hr, mode);
            BSP_LCD_DisplayChar(680, timey, 58);
            BSP_LCD_DisplayStringAt(700,timey, (uint8_t *)min, mode);
            BSP_LCD_DisplayChar(740, timey, 58);
            BSP_LCD_DisplayStringAt(760,timey, (uint8_t *)sec, mode);

            timey=timey+30;
        }
        xbound1[0]=0;
        xbound1[1]=0;
        ybound1[0]=0;
        ybound1[1]=0;

        xbound2[0]=0;
        xbound2[1]=0;
        ybound2[0]=0;
        ybound2[1]=0;

        xbound3[0]=290;
        xbound3[1]=510;
        ybound3[0]=320;
        ybound3[1]=480;

        nextframe=1;
        leftframe=5;
        rightframe=5;
        currentframe=3;

    }
    else if (frame==10){//this is the "EXIT" frame
        //this is mean to be the last thing displayed on the screen as it is the end of the program
    	//all final data is neatly displayed for the user

    	BSP_LCD_Clear(LCD_COLOR_WHITE);//needed to remove the exit button

        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

        //display total people who entered the room
        BSP_LCD_DisplayStringAt(329,30, (uint8_t *)totalstr, mode);
        itoa(totalint,prntchar,10);
        BSP_LCD_DisplayStringAt(380,56, (uint8_t *)prntchar, mode);

        //display number of people currently in the room
        BSP_LCD_DisplayStringAt(211,228, (uint8_t *)currentstr, mode);
        itoa(currentint,prntchar,10);
        BSP_LCD_DisplayStringAt(380,252, (uint8_t *)prntchar, mode);

        //display maximum number of people in the room at once
        BSP_LCD_DisplayStringAt(329,396, (uint8_t *)maxstr, mode);
        itoa(maxint,prntchar,10);
        BSP_LCD_DisplayStringAt(380,422, (uint8_t *)prntchar, mode);



    }

}

uint32_t HCSR04_Read1 ()
{
	 local_time1=0;
	 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the TRIG pin LOW
	 while(trigger1 < 2){
		 // wait for 2 us
	 }
	 trigger1 = 0;


	 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	 while(trigger1 < 10){
		 // wait for 10 us
	 }
	 trigger1 = 0;
	 HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the TRIG pin low

	 // read the time for which the pin is high

	 while (!(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7)));  // wait for the ECHO pin to go high
	 while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7))    // while the pin is high
	  {
	  local_time1++;   // measure time for which the pin is high
		 while(trigger1 < 1){

		 }
		 trigger1 = 0;
	  }
	 return local_time1;
}

uint32_t HCSR04_Read2 ()
{
	 local_time2=0;
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // pull the TRIG pin LOW
	 while(trigger2 < 2){
		 // wait for 2 us
	 }
	 trigger2 = 0;


	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	 while(trigger2 < 10){
		 // wait for 10 us
	 }
	 trigger2 = 0;
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // pull the TRIG pin low

	 // read the time for which the pin is high

	 while (!(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_0)));  // wait for the ECHO pin to go high
	 while (HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_0))    // while the pin is high
	  {
	  local_time2++;   // measure time for which the pin is high
		 while(trigger2 < 1){

		 }
		 trigger2 = 0;
	  }
	 return local_time2;
}

void TIM7_IRQHandler(){
	HAL_TIM_IRQHandler(&HALtime7);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM7){
		trigger1++;
		trigger2++;
		trigger_time++;
		if(trigger_time >= 1000000){
			//time_running = time_running + 0.001;
			time_running++;
			seconds++;
			trigger_time = 0;
			if(seconds > 59){
				seconds = 0;
				minutes++;
			}

			if(minutes > 59){
				minutes = 0;
				hours++;
			}

			if(hours > 23){
				minutes = 0;
				hours = 0;
				seconds = 0;
			}
    		printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    		fflush(stdout);
			//printf("%dH:%dM:%dS\r\n",hours,minutes,seconds);
		}
		read_delay++;
		if(read_delay >=200000){
			read_delay = 0;
			check_sensors = 1;
		}

		clear_delay++;
		if(clear_delay >=2000000){
			clear_delay = 0;
			sensor_clear = 1;
		}

	}
}

