//------------------------------------------------------------------------------------
// Hello.c
//------------------------------------------------------------------------------------
//
// Test program to demonstrate serial port I/O.  This program writes a message on
// the console using the printf() function, and reads characters using the getchar()
// function.  An ANSI escape sequence is used to clear the screen if a '2' is typed.
// A '1' repeats the message and the program responds to other input characters with
// an appropriate message.
//
// Any valid keystroke turns on the green LED on the board; invalid entries turn it off
//


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

    char choice;
    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
    printf("PRESS <ESC> OR <CTRL>+[ TO QUIT\r\n\n");

    while(1)
    {
    	choice = getchar();

    	if (choice == 27) //check for esc or ^[ to end program
    	{
    		printf("\033[2J\033[;H");
    		fflush(stdout);


    		printf("Program Halted");
    		fflush(stdout);

    		break;
    	}

    	else if ((choice >= 32) && (choice < 127)  ) //Check if character is printable
    	{
    		printf("\033[2K\r");
    		fflush(stdout);
    		printf("The keyboard character is %c.",choice);
    		fflush(stdout);
    	}

	}
}
