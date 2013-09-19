#pragma once

#include "var.h"
#include "scope.h"
#include "../common/stack.h"
#include "../common/codes.h"
#include "../common/opcode.h"

#include <map>
using namespace std;

#ifdef _DEBUG
#	define LOGF(A) printf A
#else
# 	define LOGF(A) false
#endif

class Environment {
public:
					Environment(Opcode *opcode);
					~Environment();

	int 			Execute();

private:
	byte			*mOpcodes;
	uint 			mOpPtr;

	/***** Program Stack ****
	* Variable IDs and opcode pointers are pushed
	* onto this stack. 
	*****/
	Stack<uint>		mPStack;

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
	void 			GetStackVar(Var *&var);

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
	void 			OpCmp();
	void 			OpJe();
	void 			OpJne();
	void 			OpJg();
	void 			OpJge();
	void 			OpJl();
	void 			OpJle();
};