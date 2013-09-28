#pragma once

#include <exception>
#include <stdexcept>
#include <string>
using namespace std;

/***** Exception Definitions *****
* Exceptions are created with macros to
* prevent clutter.
*****/
#define EX_BEGIN(_NAME, _TXTDEFAULT)		\
	class _NAME : public runtime_error {	\
	public: 								\
	_NAME(string txt=_TXTDEFAULT)			\
			:runtime_error(txt){}			\
	const char *what() {					\
		string str = #_NAME;				\
		str += ": ";						\
		str += runtime_error::what();		\
		return str.c_str();					\
	}

#define EX_END	\
	};


EX_BEGIN(InvalidOpException, "Invalid operation")
EX_END

EX_BEGIN(VarNotDefinedException, "Variable not defined")
EX_END

EX_BEGIN(VarAlreadyDefinedException, "Variable is already defined");
EX_END

EX_BEGIN(FuncNotDefinedException, "Function is not defined")
EX_END

EX_BEGIN(FuncAlreadyDefinedException, "Function is already defined!");
EX_END

EX_BEGIN(FileNotFoundException, "File does not exist")
EX_END

EX_BEGIN(InvalidTokenException, "Invalid token")
EX_END

EX_BEGIN(SyntaxErrorException, "Syntax error")
EX_END

EX_BEGIN(NotImplementedException, "Feature not implemented")
EX_END

EX_BEGIN(StackOverflowException, "Stack overflow")
EX_END

EX_BEGIN(StackUnderflowException, "Stack underflow")
EX_END

EX_BEGIN(NullPointerException, "Null-pointer exception")
EX_END