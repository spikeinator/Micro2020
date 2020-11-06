


#include "init.h"



int main(void){
	//Initialize the system
	Sys_Init();

	int32_t a; //result of part 1

	double b = 8;//operand for part 2
	double c = 5;//operand for part 2
	double d=0; //result of part 2

	double x = 10;
	double mul1=2;
	double div1=3;
	double add1=5;
	double pass1;
	double pass2;
	double pass3;

	double y =10;
	double div2=3;
	double mul2=2;
	double fiv=5;

	double pass4;


	printf("\033[2J\033[;H"); // Erase screen & move cursor to home position

	//Part 1
	asm("LDR r3, =0x00000006");
	asm("LDR r2, =0x00000004");
	asm("ADD r3, r3, r2");

	asm("STR r3,%0" : "=m" (a));

	printf("Part 1: 6 + 4 = %lu\r\n",a);

	asm volatile("VMLA.F64 %P[out],%P[in1],%P[in2]"
	:[out] "+w" (d)
	:[in1] "w" (b),[in2] "w" (c));

	printf("Part 2: %f x %f = %f\r\n",b,c,d);

	asm("VMUL.F64 %P[out],%P[in1],%P[in2]"
	:[out] "=w" (pass1)
	:[in1] "w" (x),[in2] "w" (mul1));

	asm("VDIV.F64 %P[out],%P[in1],%P[in2]"
	:[out] "=w" (pass2)
	:[in1] "w" (pass1),[in2] "w" (div1));

	asm("VADD.F64 %P[out],%P[in1],%P[in2]"
	:[out] "=w" (pass3)
	:[in1] "w" (pass2),[in2] "w" (add1));

	printf("Part 3: x  = %f, (2x/3)+5 = %f\r\n",x,pass3);

	asm("VDIV.F64 %P[out],%P[in1],%P[in2]"
	:[out] "=w" (pass4)
	:[in1] "w" (y),[in2] "w" (div2));

	asm("VMLA.F64 %P[out],%P[in1],%P[in2]"
		:[out] "+w" (fiv)
		:[in1] "w" (pass4),[in2] "w" (mul2));

	printf("Part 4: x  = %f, (2x/3)+5 = %f\r\n",y,fiv);
}
