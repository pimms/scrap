#include "env.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

Environment::Environment(Opcode *opcode) {
	mOpcodes = opcode->GetRaw();
	mOpPtr = 0;
	mDataDef = false;

	// Allocate the global return value 
	mGScope.Alloc(VAR_RETURN);
}

Environment::~Environment() {
	if (mOpcodes) {
		delete[] mOpcodes;
	}
}

int Environment::Execute() {
	printf("Starting execution...\n");

	while (mOpcodes[mOpPtr] != OP_EXIT) {
		Operation();
	}

	mOpPtr++;
	int exitId = GetOpcodeUint();

	if (exitId & (VAR_GLOBAL | VAR_LOCAL)) {
		Var *var = GetVarById(exitId);
		exitId = var->GetInt();
	}

	printf("Program terminated with status %i\n", exitId);
	return exitId;
}




void Environment::Operation() {
	byte op = mOpcodes[mOpPtr++];

	if (op >= 0xF0) {
		switch (op) {
			case OP_DATA_BEGIN:		OpDataBegin();	break;
			case OP_DATA_STRING:	OpDataString();	break;
			case OP_DATA_FUNC:		OpDataFunc();	break;
			case OP_DATA_END:		OpDataEnd();	break;
		}
	} else {
		if (mDataDef) {
			throw InvalidOpException("Cannot perform common operations when defining data");
		}

		if (op <= 0x1F) {
			switch (op) {
				case OP_PUSH: 		OpPush();		break;
				case OP_PUSH_DATA:	OpPushData();	break;
				case OP_POP:		OpPop();		break;
				case OP_CALL:		OpCall();		break;
				case OP_RET:		OpRet();		break;
				case OP_ALLOC:		OpAlloc();		break;
				case OP_POPMOV:		OpPopMov();		break;
				case OP_MOV: 		OpMov();		break;
				case OP_MOVI: 		OpMovI();		break;
				case OP_MOVF: 		OpMovF();		break;
				case OP_MOVS: 		OpMovS();		break;

				case OP_ADD: 		OpAdd();		break;
				case OP_SUB: 		OpSub();		break;
				case OP_MUL: 		OpMul();		break;
				case OP_DIV: 		OpDiv();		break;
				case OP_MOD: 		OpMod();		break;

				case OP_ADD_I:		OpAddI();		break;
				case OP_SUB_I:		OpSubI();		break;
				case OP_MUL_I:		OpMulI();		break;
				case OP_DIV_I:		OpDivI();		break;
				case OP_MOD_I:		OpModI();		break;

				case OP_ADD_F:		OpAddF();		break;
				case OP_SUB_F:		OpSubF();		break;
				case OP_MUL_F:		OpMulF();		break;
				case OP_DIV_F:		OpDivF();		break;

				case OP_PUSH_SCOPE:	OpPushScope();	break;
				case OP_POP_SCOPE:	OpPopScope();	break;

				case OP_EXIT: 		/* TODO */		break;
			}
		} else if (op <= 0x2F) {
			switch (op) {
				case OP_JMP:		OpJmp();		break;
				case OP_JE:			OpJe();			break;
				case OP_JNE:		OpJne();		break;
				case OP_JG:			OpJg();			break;
				case OP_JGE:		OpJge();		break;
				case OP_JL:			OpJl();			break;
				case OP_JLE:		OpJle();		break;
			}
		} else {
			// Corrupted binary
		}
	}
}


Var* Environment::GetVarById(uint id) {
	Var *ret = NULL;

	if (id & VAR_GLOBAL) {
		ret = mGScope.GetVar(id);
	} else {
		ret = mLScope.Peek()->GetVar(id);
	}

	return ret;
}

void Environment::PopStackVar(Var *&var) {
	uint varId = 0;

	varId = mPStack.Pop();
	var = GetVarById(varId);
}


int Environment::GetOpcodeInt() {
	byte *bytePtr = &((byte*)mOpcodes)[mOpPtr];

	mOpPtr += 4;

	return *(int*)bytePtr;
}

uint Environment::GetOpcodeUint() {
	byte *bytePtr = &((byte*)mOpcodes)[mOpPtr];

	mOpPtr += 4;

	return *(uint*)bytePtr;
}

float Environment::GetOpcodeFloat() {
	byte *bytePtr = &((byte*)mOpcodes)[mOpPtr];

	mOpPtr += 4;

	return *(float*)bytePtr;
}

Var* Environment::GetOpcodeVar() {
	uint id = GetOpcodeUint();
	return GetVarById(id);
}


Scope* Environment::GetCurrentScope() {
	if (!mLScope.Size()) {
		return &mGScope;
	} 
		
	return mLScope.Peek();
}


void Environment::OpPush() {
	uint varId = GetOpcodeUint();
	mPStack.Push(varId);

	LOGF(("Pushing var %x\n", varId));
}

void Environment::OpPushData() {
	// TODO
}

void Environment::OpPop() {
	mPStack.Pop();
}

void Environment::OpCall() {
	uint funcId = GetOpcodeUint();
	uint funcPos = mFunctions[funcId];

	// Push the next instruction to be executed.
	mFStack.Push(mOpPtr);
	mOpPtr = funcPos;

	Scope *localScope = new Scope();
	mLScope.Push(localScope);

	LOGF(("Calling %x\n", funcId));
}

void Environment::OpRet() {
	Scope *localScope = NULL;
	int retId = 0;

	// Copy the value of the local var
	// to the return variable.
	retId = GetOpcodeUint();
	Var *retSrc = NULL;
	if ((retSrc = GetVarById(retId))) {
		Var *retDst = GetVarById(VAR_RETURN);
		*retDst = *retSrc;
		mPStack.Push(VAR_RETURN);
	} else {
		mPStack.Push(0);
	}

	// Delete the current scope
	localScope = mLScope.Pop();
	delete localScope;

	// Pop the previous position
	mOpPtr = mFStack.Pop();

	LOGF(("Returning to %u\n", mOpPtr));
}

void Environment::OpAlloc() {
	uint varId = GetOpcodeUint();

	if (varId & VAR_GLOBAL) {
		mGScope.Alloc(varId);
	} else {
		mLScope.Peek()->Alloc(varId);
	}

	LOGF(("Allocating %x\n", varId));
}

void Environment::OpPopMov() {
	Var *source = NULL;
	Var *dest = NULL;

	PopStackVar(source);

	dest = GetOpcodeVar();
	*dest = *source;

	LOGF(("Popping Var %x into Var %x\n", source->GetId(), dest->GetId()));
}

void Environment::OpMov() {
	Var *source = NULL;
	Var *dest = NULL;

	PopStackVar(source);
	PopStackVar(dest);

	*dest = *source;
}

void Environment::OpMovI() {
	Var *dest = NULL;
	int literal = 0;

	dest = GetOpcodeVar();
	literal = GetOpcodeUint();
	
	dest->Set(literal);

	LOGF(("Var %x = %i\n", dest->GetId(), literal));
}

void Environment::OpMovF() {
	Var *dest = NULL;
	float literal = 0;

	dest = GetOpcodeVar();
	literal = GetOpcodeFloat();

	dest->Set(literal);

	LOGF(("Var %x = %f\n", dest->GetId(), literal));
}

void Environment::OpMovS() {
	// TODO
}



void Environment::OpAdd() {
	ArithmeticStack(&Var::operator+=);
}

void Environment::OpSub() {
	ArithmeticStack(&Var::operator-=);
}

void Environment::OpMul() {
	ArithmeticStack(&Var::operator*=);
}

void Environment::OpDiv() {
	ArithmeticStack(&Var::operator/=);
}

void Environment::OpMod() {
	ArithmeticStack(&Var::operator%=);
}

void Environment::ArithmeticStack(void(Var::*oper)(const Var&)) {
	Var *left = NULL;
	Var *right = NULL;

	PopStackVar(right);
	PopStackVar(left);

	(*left.*oper)(*right);
}


void Environment::OpAddI() {
	ArithmeticInt(&Var::operator+=);
}

void Environment::OpSubI() {
	ArithmeticInt(&Var::operator-=);
}

void Environment::OpMulI() {
	ArithmeticInt(&Var::operator*=);
}

void Environment::OpDivI() {
	ArithmeticInt(&Var::operator/=);
}

void Environment::OpModI() {
	ArithmeticInt(&Var::operator%=);
}

void Environment::ArithmeticInt(void(Var::*oper)(const int&)) {
	Var *var = NULL;
	int literal = 0;

	PopStackVar(var);
	literal = GetOpcodeInt();

	(*var.*oper)(literal);
}


void Environment::OpAddF() {
	ArithmeticFloat(&Var::operator+=);
}

void Environment::OpSubF() {
	ArithmeticFloat(&Var::operator-=);
}

void Environment::OpMulF() {
	ArithmeticFloat(&Var::operator*=);
}

void Environment::OpDivF() {
	ArithmeticFloat(&Var::operator/=);
}

void Environment::ArithmeticFloat(void(Var::*oper)(const float&)) {
	Var *var = NULL;
	float literal = 0.f;

	PopStackVar(var);
	literal = GetOpcodeFloat();

	(*var.*oper)(literal);
}


void Environment::OpPushScope() {
	Scope *scope = GetCurrentScope();
	scope->PushNestedScope();
}

void Environment::OpPopScope() {
	Scope *scope = GetCurrentScope();
	scope->PopNestedScope();
}


void Environment::OpJmp() {
	int dest = GetOpcodeUint();
	mOpPtr = dest;

	LOGF(("Jumping to %i\n", dest));
}

void Environment::OpJe() {
	LOGF(("Comparing ==\n"));
	CompareJump(&Var::operator==);
}

void Environment::OpJne() {
	LOGF(("Comparing !=\n"));
	CompareJump(&Var::operator!=);
}

void Environment::OpJg() {
	LOGF(("Comparing >\n"));
	CompareJump(&Var::operator>);
}

void Environment::OpJge() {
	LOGF(("Comparing >=\n"));
	CompareJump(&Var::operator>=);
}

void Environment::OpJl() {
	LOGF(("Comparing <\n"));
	CompareJump(&Var::operator<);
}

void Environment::OpJle() {
	LOGF(("Comparing <=\n"));
	CompareJump(&Var::operator<=);
}

void Environment::CompareJump(bool(Var::*cmp)(const Var&)const) {
	Var *left = NULL;
	Var *right = NULL;

	PopStackVar(right);
	PopStackVar(left);

	if ((left->*cmp)(*right)) {
		OpJmp();
	} else {
		// Ignore the op-pointer
		mOpPtr += 4;
	}
}


void Environment::OpDataBegin() {
	if (mDataDef) {
		throw InvalidOpException("Already defining data");
	}

	mDataDef = true;
}

void Environment::OpDataString() {
	// TODO
}

void Environment::OpDataFunc() {
	int funcId = 0;
	int funcPos = 0;

	funcId = GetOpcodeUint();
	funcPos = GetOpcodeUint();

	mFunctions[funcId] = funcPos;
}

void Environment::OpDataEnd() {
	if (!mDataDef) {
		throw InvalidOpException("Was not defining data");
	}

	mDataDef = false;
}