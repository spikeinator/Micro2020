/*
 * Lab4-3part1.c
 *
 *  Created on: Oct 22, 2020
 *      Author: peckn
 */


#include "init.h"


int main(void){
	//Initialize the system
	Sys_Init();

	int32_t a; //result of part 2

	int32_t b = 4;//operand for part 2
	int32_t c = 6;//operand for part 2
	int32_t d; //result of part 2

	int32_t e = 24;// x value for part 3 and part 4
	int32_t f = 2; //value in equation
	int32_t g = 3; // value in equation
	int32_t h = 5; //value in equation
	int32_t i1;// intermediate of part 3 and 4
	int32_t i2;// intermediate of part 3 and result of 4
	int32_t i3;// result of part 3



	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	//Part 1
	asm("LDR r3, =0x00000006");
	asm("LDR r2, =0x00000004");
	asm("ADD r3, r3, r2");

	asm("STR r3,%0" : "=m" (a));

	printf("Part 1: 6 + 4 = %lu\r\n",a);

	//Part 2
	asm("MUL %[out],%[in1],%[in2]"
	:[out] "=r" (d)
	:[in1] "r" (b),[in2] "r" (c));

	printf("Part 2: %lu x %lu = %lu\r\n",a,b,c);

	//Part 3
	asm("MUL %[out],%[in1],%[in2]"
	:[out] "=r" (i1)
	:[in1] "r" (f),[in2] "r" (e));

	asm("SDIV %[out],%[in1],%[in2]"
	:[out] "=r" (i2)
	:[in1] "r" (i1),[in2] "r" (g));

	asm("ADD %[out],%[in1],%[in2]"
	:[out] "=r" (i3)
	:[in1] "r" (i2),[in2] "r" (h));

	printf("Part 3: x  = %lu, (2x/3)+5 = %lu\r\n",e,i3);

	//Part 4
	asm("SDIV %[out],%[in1],%[in2]"
	:[out] "=r" (i1)
	:[in1] "r" (e),[in2] "r" (g));

	asm("MLA %[out],%[in1],%[in2],%[in3]"
	:[out] "=r" (i2)
	:[in1] "r" (i1),[in2] "r" (f),[in3] "r" (h));

	printf("Part 4: x  = %lu, (2x/3)+5 = %lu",e,i2);

}
