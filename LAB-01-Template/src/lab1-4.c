/*
 * lab1-4.c
 *
 *  Created on: Sep 9, 2020
 *      Author: peckn
 */


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "stm32f769xx.h"
#include "hello.h"

#include<stdint.h>

void setupmazeframe();//Function to create a square of walls and put wall locations in the map
void setupmazewalls();//Function to create walls within the maze and put those locations on the map
void printmap(); //Prints map for testing

int i = 0;
int j = 0;
int x;
int y;
char choice;
int map[12][12]; //Array to keep track of player and walls
//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
	Sys_Init();

	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
	fflush(stdout);

	printf("\033[0m");

	printf("\033[s");
	fflush(stdout);

	setupmazeframe();
	setupmazewalls();
	printmap();

	printf("\033[2;2H\033[s\033[46m \033[0m\033[15;15H");
	fflush(stdout);

	while(1)
	{
		choice = getchar();

		//check for "w" input and move player up
		if((choice == 119) || (choice == 87)){
			printf("\033[u \033[D");
			fflush(stdout);

			printf("\033[A\033[s\033[46m \033[0m");
			fflush(stdout);

			printf("\033[15;15H");
			fflush(stdout);
		}

		//check for "d" input and move player right
		else if((choice == 68) || (choice == 100)){
			printf("\033[u \033[D");
			fflush(stdout);

			printf("\033[C\033[s\033[46m \033[0m");
			fflush(stdout);

			printf("\033[15;15H");
			fflush(stdout);
		}

		//Check for "s" input and move player down
		else if((choice == 83) || (choice == 115)){
			printf("\033[u \033[D");
			fflush(stdout);

			printf("\033[B\033[s\033[46m \033[0m");
			fflush(stdout);

			printf("\033[15;15H");
			fflush(stdout);
		}

		//Check for "a" input and move player left
		else if((choice == 65) || (choice == 97)){
			printf("\033[u \033[D");
			fflush(stdout);

			printf("\033[D\033[s\033[46m \033[0m");
			fflush(stdout);

			printf("\033[15;15H");
			fflush(stdout);
		}

	}
}

void setupmazeframe(){
	printf("\033[u");
	fflush(stdout);

	for(i=0; i<12; i++ ){
		map[0][i] = 1;
		printf("\033[44m \033[0m");
		fflush(stdout);
	}

	printf("\033[u\033[11B");
	fflush(stdout);

	for(i=0; i<12; i++ ){
		map[11][i] = 1;
		printf("\033[44m \033[0m");
		fflush(stdout);
	}

	printf("\033[u\033[B");
	fflush(stdout);

	for(i=0; i<11; i++ ){
		map[i][0] = 1;
		printf("\033[44m \033[0m");
		fflush(stdout);

		printf("\033[B\033[D");
		fflush(stdout);
	}


	printf("\033[u\033[B\033[11C");
	fflush(stdout);

	for(i=0; i<11; i++ ){
		map[i][11] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[12;11H");
	fflush(stdout);
	printf("\033[43m \033[0m");
	fflush(stdout);

	map[11][10] = 2;

	printf("\033[10C");
	fflush(stdout);
}

void setupmazewalls(){
	printf("\033[u");
	fflush(stdout);

	printf("\033[3;2H");
	fflush(stdout);

	for(i=1;i<6;i++){
		map[2][i] = 1;
		printf("\033[44m \033[0m");
		fflush(stdout);
	}

	printf("\033[2;8H");
	fflush(stdout);
	for(i=1;i<10;i++){
		map[i][7] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[3;10H");
	fflush(stdout);
	for(i=2;i<11;i++){
		map[i][9] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[5;3H");
	fflush(stdout);
	for(i=4;i<10;i++){
		map[i][2] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[5;5H");
	fflush(stdout);
	for(i=4;i<7;i++){
		map[i][4] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[9;5H");
	fflush(stdout);
	for(i=8;i<11;i++){
		map[i][4] = 1;
		printf("\033[44m \033[0m\033[B\033[D");
		fflush(stdout);
	}

	printf("\033[5;4H");
	fflush(stdout);
	map[4][3] = 1;
	printf("\033[44m \033[0m");
	fflush(stdout);

	printf("\033[5;7H");
	fflush(stdout);
	map[4][6] = 1;
	printf("\033[44m \033[0m");
	fflush(stdout);

	printf("\033[7;6H");
	fflush(stdout);
	map[6][5] = 1;
	printf("\033[44m \033[0m");
	fflush(stdout);

	printf("\033[9;7H");
	fflush(stdout);
	map[8][6] = 1;
	printf("\033[44m \033[0m");
	fflush(stdout);

	printf("\033[10;7H");
	fflush(stdout);
	map[9][6] = 1;
	printf("\033[44m \033[0m");
	fflush(stdout);
}

void printmap(){
	printf("\033[;15H");
	fflush(stdout);
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			printf("%d",map[i][j]);
			fflush(stdout);
		}
		printf("\033[12D\033[B");
		fflush(stdout);
	}
}
