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
		RegisterFunction(parser, &Print,	"print",	1);
	}



	Var* ScrapStdCall(void *func, int pCount, Var **params) {
		Var *retVal = NULL;

		for (int i = pCount-1; i >= 0; i--) {
			void *paramPtr = params[i];
			__asm 
			{
				mov		eax, paramPtr
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

		return retVal;
	}

	int GetParamCount(uint funcId) {
		return stdFuncMap[funcId].params;
	}

	Var* CallStdFunc(uint id, int argCount, Var **params) {
		StdFunc f = stdFuncMap[id];
		return ScrapStdCall(f.func, f.params, params);
	}



	Var* Print(Var *var) {
		printf("%i\n", var->GetInt());
		return NULL;
	}
}