#pragma once

#include "var.h"
#include "../common/codes.h"
#include "../common/stack.h"

#include <map>
#include <stdlib.h>

using namespace std;

class Scope {
public:
					~Scope();

	bool 			Alloc(uint id);
	Var* 			GetVar(uint id);

	void			PushNestedScope();
	void			PopNestedScope();

private:
	map<uint,Var*>	mVars;
	Stack<Scope*>	mNested;
};