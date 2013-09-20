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
	opsCalc.AddByte(OP_EXIT)->AddInt(VAR_GLOBAL  | 1);

	/* Example program calling a function taking a parameter.
	 * The parameter is multiplied by 14 and returned. The
	 * program then exits with the return value (5*14=70).
	 */
	Opcode opsFunc;
	opsFunc.AddByte(OP_DATA_BEGIN);
	opsFunc.AddByte(OP_DATA_FUNC)->AddUint(1)->AddUint(45);
	opsFunc.AddByte(OP_DATA_END);
	opsFunc.AddByte(OP_ALLOC)->AddUint(VAR_GLOBAL | 1);
	opsFunc.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL  | 1)->AddInt(5);
	opsFunc.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL  | 1);
	opsFunc.AddByte(OP_CALL)->AddUint(1);
	opsFunc.AddByte(OP_POPMOV)->AddUint(VAR_GLOBAL| 1);
	opsFunc.AddByte(OP_EXIT)->AddUint(VAR_GLOBAL  | 1);

	opsFunc.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL  | 1);
	opsFunc.AddByte(OP_POPMOV)->AddUint(VAR_LOCAL | 1);
	opsFunc.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL  | 2);
	opsFunc.AddByte(OP_MOVI)->AddUint(VAR_LOCAL   | 2)->AddInt(14);
	opsFunc.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 1);
	opsFunc.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 2);
	opsFunc.AddByte(OP_MUL);
	opsFunc.AddByte(OP_POP);
	opsFunc.AddByte(OP_RET)->AddUint(VAR_LOCAL | 1);

	{
		printf("Calculation program:\n");
		Environment env(&opsCalc);
		env.Execute();
		printf("\n\n");
	}
	{
		printf("Function program:\n");
		Environment env(&opsFunc);
		env.Execute();
		printf("\n\n");
	}
	
	getchar();
	return 0;
}