#include "IndexList.h"
#include "Variable.h"
#include "Function.h"


namespace scrap {


FunctionList::FunctionList()
	:	_inheritCount(0)
{

}

FunctionList::~FunctionList()
{
	for (int i=_inheritCount; i<_functions.size(); i++) {
		delete _functions[i];
	}
}

void FunctionList::AddFunction(Function *function)
{
	_functions.push_back(function);
}

Function* FunctionList::GetFunction(unsigned id) const 
{
	if (id >= _functions.size())
		THROW(IndexOutOfRangeException,
		"Attempted to access field with index out of range");
	return _functions[id];
}

unsigned FunctionList::GetFunctionCount() const
{
	return _functions.size();
}


}
