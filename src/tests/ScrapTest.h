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
#include "../MemberList.h"
#include "../BinaryFile.h"

using namespace scrap;

// All methods created take a single DOUBLE as an argument, and returns an INT. 
Method CreateMethod(Class *c, MethodType type = METHOD_NORMAL);

