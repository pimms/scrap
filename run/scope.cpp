#include "scope.h"
#include <stdlib.h>

bool Scope::Alloc(uint id) {
#ifdef _DEBUG
	if (GetVar(id) != NULL) {
		return false;
	}
#endif

	Var *var = new Var(id);
	mVars[id] = var;

	return true;
}

Var* Scope::GetVar(uint id) {
	if (mVars.count(id) == 0) {
		return NULL;
	}

	return mVars[id];
}