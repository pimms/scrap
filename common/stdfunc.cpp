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
		RegisterFunction(parser, (void*)&Print,	"print",	1);
	}



	Var* ScrapStdCall(void *func, int pCount, Var **params) {
		Var *retVal = NULL;

		for (int i = pCount-1; i >= 0; i--) {
			void *paramPtr = params[i];
			
			#ifdef _MSC_VER
			__asm 
			{
				mov		eax, paramPtr
				push	eax
			}
			#elif defined(__GNUC__)
			asm("push $0\n" : "=r" (paramPtr));
			#endif
		}

		#ifdef _MSC_VER
		__asm
		{
			call	func
			mov		retVal, eax
		}
		#elif defined(__GNUC__) 
		asm volatile("call *0\n" :: "r"(func) );
		#endif

		for (int i=0; i<pCount; i++) {
			#ifdef _MSC_VER
			__asm
			{
				pop		ebx
			}
			#elif defined(__GNUC__)
			__asm("pop %bx");
			#endif
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