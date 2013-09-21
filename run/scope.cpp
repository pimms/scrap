#include "scope.h"
#include <stdlib.h>


Scope::~Scope() {
	map<uint,Var*>::iterator it;

	for (it = mVars.begin(); it != mVars.end(); it++) {
		delete it->second;
	}
}


bool Scope::Alloc(uint id) {
	Scope *nested = mNested.Peek();
	if (nested) {
		return nested->Alloc(id);
	}

	if (GetVar(id) != NULL) {
		return false;
	}

	Var *var = new Var(id);
	mVars[id] = var;

	return true;
}

Var* Scope::GetVar(uint id) {
	for (int i=mNested.Size()-1; i>=0; i--) {
		Var *var = mNested.Peek(i)->GetVar(id);
		if (var) {
			return var;
		}
	}

	if (mVars.count(id) == 0) {
		return NULL;
	}

	return mVars[id];
}



void Scope::PushNestedScope() {
	Scope *nested = new Scope();
	mNested.Push(nested);
}

void Scope::PopNestedScope() {
	if (mNested.Size() > 0) {
		Scope *nested = mNested.Pop();
		delete nested;
	} else {
		throw UnderflowException();
	}
}