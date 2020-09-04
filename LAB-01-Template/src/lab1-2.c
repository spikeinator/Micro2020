/*
 * lab1-2.c
 *
 *  Created on: Sep 2, 2020
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

    char choice = 0;
    //char counter = 1;
    //char arr[13];
   // int i = 1;

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

    printf("\033[44;33m"); //Changes color of text and background
    fflush(stdout);

    printf("\033[12;H\033[s");
    fflush(stdout);
    //sets scrolling window for lines 12-24
    printf("\033[12;24r");
    fflush(stdout);

    printf("\033[;H");
    fflush(stdout);

    printf("\033[2;25H"); //Moves Cursor to Center Text
    fflush(stdout);
    printf("PRESS <ESC> OR <CTRL>+[ TO QUIT\r\n\n");



    while(1)
    {
    	choice = getchar();


    	if (choice == 27) //check for esc or ^[ to end program
    	{
    		printf("\a"); //Should have terminal beep
    		fflush(stdout);
    		printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    		fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n

    		printf("\033[33;44mProgram Halted");
    		fflush(stdout);
    		break;
    	}

    	else if ((choice >= 32) && (choice < 127)  ) //Check if character is printable
    	{
    		//printf("\033[33mThe keyboard character is \033[31m%c\033[33m.\r\n",choice);
    		printf("\033[6;H");
    		fflush(stdout);
    		printf("\033[2K\r"); //Erase the current line
    		fflush(stdout);
    		printf("\033[33mThe keyboard character is \033[31m%c\033[33m.",choice);
      		fflush(stdout);
    	}

    	else{
		printf("\a"); //Should have terminal beep
    		fflush(stdout);
   			printf("\033[u");
    		fflush(stdout);
    		printf("\033[33;5mThe keyboard character $%x is \033[4m'not printable'\033[24m.\033[25m",choice);
    		fflush(stdout);
    		printf("\033[u");
    		fflush(stdout);
   			printf("\033D");
   			fflush(stdout);

   			printf("\033[s");
   			fflush(stdout);
    	}
    	/*
    	else{
    		printf("\a"); //Should have terminal beep
    		if (counter <14)
    		{	arr[counter-1] = choice;
    			printf("\033[u");
    			fflush(stdout);
    			printf("\033[33;5mThe keyboard character $%x is \033[4m'not printable'\033[24m.\033[25m",arr[counter-1]);
    			fflush(stdout);

    			printf("\033[u");
    			fflush(stdout);

    			printf("\033[B");
    			fflush(stdout);

    			printf("\033[s");
    			fflush(stdout);

    			counter ++;
    		}

    		else{
    			printf("\033[11;H");
    			fflush(stdout);
    			for(i=1; i!=13; i++){
    				printf("\033[B\033[2K\033[80D");
    				fflush(stdout);

    				printf("\033[33;5mThe keyboard character $%x is \033[4m'not printable'\033[24m.\033[25m",arr[i]);
    				fflush(stdout);
    				arr[i-1] = arr[i];


    			}
				printf("\033[B\033[2K\033[80D");
				fflush(stdout);
				arr[12] = choice;
				printf("\033[33;5mThe keyboard character $%x is \033[4m'not printable'\033[24m.\033[25m",arr[12]);
				fflush(stdout);

    		}

    	}*/


	}
}
