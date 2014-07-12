#pragma once

#include "gtest/gtest.h"
#include "../Scrap.h"
#include "../Object.h"
#include "../Variable.h"
#include "../Class.h"
#include "../Method.h"
#include "../MethodInvocation.h"
#include "../Heap.h"
#include "../Stack.h"
#include "../IndexList.h"
#include "../BinaryFile.h"
#include "../Bytecode.h"
#include "../ProgramParser.h"
#include "../Program.h"
#include "../Debug.h"

using namespace scrap;

// Creates a method which take a single DOUBLE as an argument, and returns an INT. 
Method* CreateMethod(Class *c, MethodType type = METHOD_NORMAL);

// No arguments, returns void
Method* CreateVoidMethod(Class *c, MethodType type = METHOD_NORMAL);
