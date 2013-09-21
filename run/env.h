#pragma once

#include "var.h"
#include "scope.h"
#include "../common/stack.h"
#include "../common/codes.h"
#include "../common/opcode.h"

#include <map>
#include <string>
#include <exception>
using namespace std;

#ifdef _DEBUG
#	define LOGF(A) printf("[%i]: ",mOpPtr); printf A
#else
# 	define LOGF(A) false
#endif

class Environment {
public:
					Environment(Opcode *opcode);
					~Environment();

	int 			Execute();

private:
	/***** Operation codes *****
	* mOpcodes holds the opcodes, and 
	* mOpPtr holds the index of the current 
	* operation to be executed.
	*****/
	byte			*mOpcodes;
	uint 			mOpPtr;


	bool			mDataDef;

	/***** Program Stack *****
	* Variable IDs are pushed onto this stack. 
	*****/
	Stack<uint>		mPStack;

	/***** Function stack *****
	* Previous opcode positions are pushed onto
	* this stack when calling a function. The value
	* is popped and moved to when the function returns.
	*****/
	Stack<uint>		mFStack;

	/***** Global Scope *****
	* Global variable scope. 
	*****/
	Scope 			mGScope;

	/***** Local Scopes *****
	* A new Scope instance is pushed onto
	* the stack for each function call.
	*****/
	Stack<Scope*> 	mLScope;

	/***** Function Map *****
	* The opcode location of all defined 
	* functions.
	*****/
	map<uint,uint>	mFunctions;

	void 			Operation();

	Var* 			GetVarById(uint id);
	void 			PopStackVar(Var *&var);

	uint 			GetOpcodeInt();
	Var* 			GetOpcodeVar();

	void 			OpPush();
	void 			OpPushData();
	void 			OpPop();
	void 			OpCall();
	void 			OpRet();
	void 			OpAlloc();
	void 			OpPopMov();
	void 			OpMov();
	void 			OpMovI();
	void 			OpMovF();
	void 			OpMovS();

	void 			OpAdd();
	void 			OpSub();
	void 			OpMul();
	void 			OpDiv();
	void 			OpMod();

	void 			OpJmp();
	void 			OpJe();
	void 			OpJne();
	void 			OpJg();
	void 			OpJge();
	void 			OpJl();
	void 			OpJle();
	void			CompareJump(bool(Var::*cmp)(const Var&)const);

	void			OpDataBegin();
	void			OpDataString();
	void			OpDataFunc();
	void			OpDataEnd();
};



/***** Exception Definitions *****
* Exceptions are created with macros to
* prevent clutter.
*****/
#define EX_BEGIN(_NAME, _TXTDEFAULT)		\
	class _NAME : public exception {		\
	public:  string mTxt;					\
	_NAME(string txt=_TXTDEFAULT)			\
			{  mTxt = txt; }				\
	virtual ~_NAME() throw() {}				\
	using std::exception::what; 			\
	const char* what()						\
			{ return mTxt.c_str(); }


#define EX_END	\
	};


EX_BEGIN(InvalidOpException, "Invalid operation")
EX_END

EX_BEGIN(VarNotDefined, "Variable not defined")
EX_END

EX_BEGIN(FuncNotDefined, "Function is not defined")
EX_END