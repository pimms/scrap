#pragma once

#include "../run/var.h"
#include "../compiler/parser.h"

/***** stdfunc.h *****
* Contains maping between opcode-functions and 
* libc/stdlibc++/custom functions.
*
* All Scrap-callable C-functions must have a Var* 
* return value, and zero or more Var* parameters.
* Example:
*		Var* f1();
*		Var* f2(Var*,Var*);
*****/
namespace ScrapStd {
	void		RegisterFunctions(Parser *parser);
	Var*		CallStdFunc(uint id, Var *var, ...);


	Var*		TestFunc(Var *var1, Var *var2);
	Var*		Print(Var *var);
};