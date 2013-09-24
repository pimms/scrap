#pragma once

#include <exception>
#include <string>
using namespace std;

/***** Exception Definitions *****
* Exceptions are created with macros to
* prevent clutter.
*****/
#define EX_BEGIN(_NAME, _TXTDEFAULT)		\
	class _NAME : public runtime_error {	\
	public:  string mTxt;					\
	_NAME(string txt=_TXTDEFAULT)			\
			:runtime_error(txt){}			\
	virtual ~_NAME() throw() {}				\
	using std::exception::what; 			\


#define EX_END	\
	};


EX_BEGIN(InvalidOpException, "Invalid operation")
EX_END

EX_BEGIN(VarNotDefined, "Variable not defined")
EX_END

EX_BEGIN(FuncNotDefined, "Function is not defined")
EX_END

EX_BEGIN(FileNotFoundException, "File does not exist")
EX_END

EX_BEGIN(InvalidTokenException, "Invalid token")
EX_END

EX_BEGIN(SyntaxErrorException, "Syntax error")
EX_END