#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "run/env.h"
#include "common/opcode.h"

int main(int argc, char *argv[]) {
	/* Example program calculating (10 * 4) / 3
	 */
	Opcode opsCalc;
	opsCalc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 1);
	opsCalc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 1)->AddInt(4);
	opsCalc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 2);
	opsCalc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 2)->AddInt(10);
	opsCalc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 3);
	opsCalc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 3)->AddInt(3);
	opsCalc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 1);
	opsCalc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 2);
	opsCalc.AddByte(OP_MUL);
	opsCalc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 3);
	opsCalc.AddByte(OP_DIV);
	opsCalc.AddByte(OP_EXIT)->AddInt(0);

	Environment env(&opsCalc);
	int retval = env.Execute();

	printf("Program terminated with status %i.\n", retval);
	getchar();

	return retval;
}