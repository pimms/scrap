#pragma once

#include "var.h"
#include "../common/codes.h"

#include <map>
#include <stdlib.h>
using namespace std;

class Scope {
public:
					~Scope();

	bool 			Alloc(uint id);
	Var* 			GetVar(uint id);

private:
	map<uint,Var*>	mVars;
};