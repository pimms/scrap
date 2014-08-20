#pragma once

#include "gtest/gtest.h"
#include "../Scrap.h"
#include "../Variable.h"
#include "../Function.h"
#include "../FunctionInvocation.h"
#include "../Stack.h"
#include "../IndexList.h"
#include "../BinaryFile.h"
#include "../Bytecode.h"
#include "../ProgramParser.h"
#include "../Program.h"
#include "../Debug.h"

using namespace scrap;

// Creates a method which take a single DOUBLE as an argument, and returns an INT. 
Function* CreateFunction();

// No arguments, returns void
Function* CreateVoidFunction();
