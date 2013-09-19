#include "env.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

Environment::Environment(Opcode *opcode) {
	mOpcodes = opcode->GetRaw();
	mOpPtr = 0;
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

	printf("Execution finished\n");
	return 0;
}




void Environment::Operation() {
	byte op = mOpcodes[mOpPtr++];

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

			case OP_EXIT: 		/* TODO */		break;
		}
	} else if (op <= 0x2F) {
		// Jump and comparisons
	} else {
		// Corrupted binary
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

void Environment::GetStackVar(Var *&var) {
	int varId = 0;

	varId = mPStack.Pop();
	var = GetVarById(varId);
}

uint Environment::GetOpcodeInt() {
	char *bytePtr = &((char*)mOpcodes)[mOpPtr];

	mOpPtr += 4;

	return *(uint*)bytePtr;
}

Var* Environment::GetOpcodeVar() {
	uint id = GetOpcodeInt();
	return GetVarById(id);
}



void Environment::OpPush() {
	uint varId = GetOpcodeInt();
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
	uint funcId = GetOpcodeInt();
	uint funcPos = mFunctions[funcId];

	mPStack.Push(mOpPtr);
	mOpPtr = funcPos;

	Scope *localScope = new Scope();
	mLScope.Push(localScope);

	LOGF(("Calling %x\n", funcId));
}

void Environment::OpRet() {
	mOpPtr = mPStack.Pop();

	Scope *localScope = mLScope.Pop();
	delete localScope;

	LOGF(("Returning to %u\n", mOpPtr));
}

void Environment::OpAlloc() {
	uint varId = GetOpcodeInt();

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

	GetStackVar(source);

	dest = GetOpcodeVar();
	*dest = *source;
}

void Environment::OpMov() {
	Var *source = NULL;
	Var *dest = NULL;

	GetStackVar(source);
	GetStackVar(dest);

	*dest = *source;
}

void Environment::OpMovI() {
	uint destId = 0;
	Var *dest = NULL;
	int literal = 0;

	destId = GetOpcodeInt();
	literal = GetOpcodeInt();
	
	dest = GetVarById(destId);
	dest->Set(literal);

	LOGF(("Var %x = %i\n", destId, literal));
}

void Environment::OpMovF() {
	Var *dest = NULL;
	float literal = 0;

	literal = mPStack.Pop();
	GetStackVar(dest);

	dest->Set(literal);

	LOGF(("Var %x = %f\n", dest->GetId(), literal));
}

void Environment::OpMovS() {
	// TODO
}



void Environment::OpAdd() {
	Var *left = NULL;
	Var *right = NULL;
	
	GetStackVar(right);
	GetStackVar(left);

	*left += *right;	
	mPStack.Push(left->GetId());

	LOGF(("Var %x += Var %x = %i\n", left->GetId(), right->GetId(), left->GetInt()));
}

void Environment::OpSub() {
	Var *left = NULL;
	Var *right = NULL;
	
	GetStackVar(right);
	GetStackVar(left);

	*left -= *right;	
	mPStack.Push(left->GetId());

	LOGF(("Var %x -= Var %x = %i\n", left->GetId(), right->GetId(), left->GetInt()));
}

void Environment::OpMul() {
	Var *left = NULL;
	Var *right = NULL;
	
	GetStackVar(right);
	GetStackVar(left);

	*left *= *right;
	mPStack.Push(left->GetId());

	LOGF(("Var %x *= Var %x = %i\n", left->GetId(), right->GetId(), left->GetInt()));
}

void Environment::OpDiv() {
	Var *left = NULL;
	Var *right = NULL;
	
	GetStackVar(right);
	GetStackVar(left);

	*left /= *right;	
	mPStack.Push(left->GetId());

	LOGF(("Var %x /= Var %x = %i\n", left->GetId(), right->GetId(), left->GetInt()));
}

void Environment::OpMod() {
	Var *left = NULL;
	Var *right = NULL;
	
	GetStackVar(right);
	GetStackVar(left);

	*left %= *right;	
	mPStack.Push(left->GetId());

	LOGF(("Var %x %%= Var %x\n", left->GetId(), right->GetId()));
}


void Environment::OpJmp() {

}

void Environment::OpCmp() {

}

void Environment::OpJe() {

}

void Environment::OpJne() {

}

void Environment::OpJg() {

}

void Environment::OpJge() {

}

void Environment::OpJl() {

}

void Environment::OpJle() {

}