#ifdef _WIN32 
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "run/env.h"
#include "common/opcode.h"

int failed = 0;
int total = 0;
void TestProgram(int(*testFunc)(), int expected, const char *desc) {
	int result = testFunc();

	if (result != expected) {
		failed++;
		printf("[FAILED]: %s\n", desc);
		getchar();
	} else {
		printf("[ok]: %s\n", desc);
	}

	total++;
}

int BasicCalculation() {
	/* Example program calculating (10 * 4) / 3
	 */
	Opcode calc;
	calc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 1);
	calc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 1)->AddInt(4);
	calc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 2);
	calc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 2)->AddInt(10);
	calc.AddByte(OP_ALLOC)->AddInt(VAR_GLOBAL | 3);
	calc.AddByte(OP_MOVI)->AddInt(VAR_GLOBAL  | 3)->AddInt(3);
	calc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 1);
	calc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 2);
	calc.AddByte(OP_MUL);
	calc.AddByte(OP_PUSH)->AddInt(VAR_GLOBAL  | 3);
	calc.AddByte(OP_DIV);
	calc.AddByte(OP_EXIT)->AddInt(VAR_GLOBAL  | 1);

	Environment env(&calc);
	return env.Execute();
}

int BasicFunction() {
	/* Example program calling a function taking a parameter.
	 * The parameter is multiplied by 14 and returned. The
	 * program then exits with the return value (5*14=70).
	 */
	Opcode func;
	func.AddByte(OP_DATA_BEGIN);
	func.AddByte(OP_DATA_FUNC)->AddUint(1)->AddUint(45);
	func.AddByte(OP_DATA_END);
	func.AddByte(OP_ALLOC)->AddUint(VAR_GLOBAL | 1);
	func.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL  | 1)->AddInt(5);
	func.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL  | 1);
	func.AddByte(OP_CALL)->AddUint(1);
	func.AddByte(OP_POPMOV)->AddUint(VAR_GLOBAL| 1);
	func.AddByte(OP_EXIT)->AddUint(VAR_GLOBAL  | 1);

	func.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL  | 1);
	func.AddByte(OP_POPMOV)->AddUint(VAR_LOCAL | 1);
	func.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL  | 2);
	func.AddByte(OP_MOVI)->AddUint(VAR_LOCAL   | 2)->AddInt(14);
	func.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 1);
	func.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 2);
	func.AddByte(OP_MUL);
	func.AddByte(OP_POP);
	func.AddByte(OP_RET)->AddUint(VAR_LOCAL | 1);

	Environment env(&func);
	return env.Execute();
}

int Comparisons() {
	/* Example program running a lot of comparisons. No
	 * comparison should ever succeed, and the program will
	 * terminate with code 0 on success, and 1 on failure.
	 */
	Opcode cmp;
	cmp.AddByte(OP_ALLOC)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL | 1)->AddInt(5);

	cmp.AddByte(OP_ALLOC)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL | 2)->AddInt(4);

	// Comparison of less
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JL)->AddUint(137);

	// Comparison of less or equal
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JLE)->AddUint(137);

	// Comparison of equality
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JE)->AddUint(137);

	// Comparison of inequality
	cmp.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL | 2)->AddInt(5);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JNE)->AddUint(137);

	// Comparison of greater
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JG)->AddUint(137);

	// Comparison of greater or equal
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	cmp.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 2);
	cmp.AddByte(OP_JGE)->AddUint(132);
	cmp.AddByte(OP_JMP)->AddUint(137);

	cmp.AddByte(OP_EXIT)->AddInt(0);
	cmp.AddByte(OP_EXIT)->AddInt(1);

	Environment env(&cmp);
	return env.Execute();
}

int Iteration() {
	/* Sample program counting from 1 to 10.
	 */
	Opcode count;
	count.AddByte(OP_DATA_BEGIN);
	count.AddByte(OP_DATA_FUNC)->AddUint(1)->AddUint(45);
	count.AddByte(OP_DATA_END);
	count.AddByte(OP_ALLOC)->AddUint(VAR_GLOBAL | 1);
	count.AddByte(OP_MOVI)->AddUint(VAR_GLOBAL | 1)->AddInt(10);
	count.AddByte(OP_PUSH)->AddUint(VAR_GLOBAL | 1);
	count.AddByte(OP_CALL)->AddUint(1);
	count.AddByte(OP_POPMOV)->AddUint(VAR_GLOBAL | 1);
	count.AddByte(OP_EXIT)->AddUint(VAR_GLOBAL | 1);

	// Function entry
	count.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL | 1);
	count.AddByte(OP_POPMOV)->AddUint(VAR_LOCAL | 1);
	count.AddByte(OP_ALLOC)->AddUint(VAR_LOCAL | 2);

	// Loop begin
	count.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 2);
	count.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 1);
	count.AddByte(OP_JGE)->AddUint(92);

	// Inner loop
	count.AddByte(OP_PUSH_SCOPE);
	count.AddByte(OP_PUSH)->AddUint(VAR_LOCAL | 2);
	count.AddByte(OP_ADD_I)->AddInt(1);
	count.AddByte(OP_POP);
	count.AddByte(OP_POP_SCOPE);
	count.AddByte(OP_JMP)->AddUint(60);

	// Loop end
	count.AddByte(OP_RET)->AddUint(VAR_LOCAL | 2);

	Environment env(&count);
	return env.Execute();
}


int main(int argc, char *argv[]) {
	TestProgram(&BasicCalculation, 13, "Calculate (4*10/3)");
	TestProgram(&BasicFunction, 70, "Basic function call");
	TestProgram(&Comparisons, 0, "Basic comparison");
	TestProgram(&Iteration, 10, "Iteration using functions");
	
	printf("\n\n%i tests completed.\n", total);
	if (failed == 0) {
		printf("No tests failed!\n");
	} else {
		printf("%i / %i tests failed\n", failed, total);
	}

#	ifdef _WIN32
		_CrtDumpMemoryLeaks();
		getchar();
#	endif

	return 0;
}