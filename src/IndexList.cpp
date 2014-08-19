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

void FunctionList::AddFunction(Function *method)
{
	_functions.push_back(method);
}

Function* FunctionList::GetFunction(unsigned id) const 
{
	if (id >= _functions.size())
		THROW(IndexOutOfRangeException,
		"Attempted to access field with index out of range");
	return _functions[id];
}


}
