#pragma once

#include "../run/var.h"
#include "../compiler/parser.h"

/***** stdfunc.h *****
* Contains maping between opcode-functions and 
* libc/stdlibc++/custom functions.
*****/
class StdFunc {
public:
	static void		RegisterFunctions(Parser *parser);

	static void		stdPrint(Var *var);
};