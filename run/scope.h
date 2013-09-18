#pragma once

#include "var.h"
#include <map>
#include <stdlib.h>
using namespace std;

class Scope {
public:
	bool 			Alloc(uint id);
	Var* 			GetVar(uint id);

private:
	map<uint,Var*>	mVars;
};