#include "stdfunc.h"
#include "../compiler/func.h"

#include <stdarg.h>

namespace ScrapStd {
	struct StdFunc {
		int params;
		void *func;
	};

	map<uint,StdFunc> stdFuncMap;


	void RegisterFunction(Parser *parser, void *func, string name, int paramCount) {
		FunctionSignature funcSign(name, paramCount);

		uint id = parser->RegisterStdFunction(funcSign);

		StdFunc f;
		f.params = paramCount;
		f.func = func;
		stdFuncMap[id] = f;
	}

	void RegisterFunctions(Parser *parser) {
		RegisterFunction(parser, &TestFunc,	"test",		2);
		RegisterFunction(parser, &Print,	"print",	1);
	}



	Var* ScrapStdCall(void *func, int pCount, Var **params) {
		Var *retVal = NULL;

		for (int i = pCount-1; i >= 0; i--) {
			__asm 
			{
				mov		eax, params[i]
				push	eax
			}
		}

		__asm
		{
			call	func
			mov		retVal, eax
		}

		for (int i=0; i<pCount; i++) {
			__asm
			{
				pop		ebx
			}
		}
	}

	Var* CallStdFunc(uint id, Var *arg0, ...) {
		StdFunc f = stdFuncMap[id];
		
		Var **params = new Var*[f.params?f.params:1];
		params[0] = arg0;

		if (f.params > 1) {
			va_list args;
			va_start(args, arg0);

			for (int i=1; i<=f.params; i++) {
				params[i] = va_arg(args, Var*);
			}

			va_end(args);
		} 

		Var *returnValue = ScrapStdCall(f.func, f.params, params);

		delete[] params;

		return returnValue;
	}



	Var* TestFunc(Var *var1, Var *var2) {
		printf("%s, %s\n", var1->GetString(), var2->GetString());
		return NULL;
	}

	Var* Print(Var *var) {
		printf("-------std print --------\n");
		return NULL;
	}
}