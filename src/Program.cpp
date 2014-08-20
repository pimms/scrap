#include "Program.h"
#include "IndexList.h"
#include "Function.h"
#include "FunctionInvocation.h"
#include "Scrap.h"

namespace scrap {

Program::Program(FunctionList *funcList, unsigned mainFuncIndex)
	:	_mainFunction(NULL),
		_funcList(funcList)
{ 
	_funcList = funcList;

	if (_funcList->GetFunctionCount() <= 0) {
		THROW(InvalidArgumentException, "No functions in assigned FunctionList");
	}

	if (mainFuncIndex >= _funcList->GetFunctionCount()) {
		THROW(InvalidArgumentException, "Invalid main-function index");
	}

	_mainFunction = _funcList->GetFunction(mainFuncIndex);
}

Program::~Program()
{

}


void Program::Execute()
{
	FunctionInvocation main(_mainFunction, NULL);
	main.Execute();
}


FunctionList* Program::GetFunctionList() const
{
	return _funcList;
}

Function* Program::GetMainFunction() const
{
	return _mainFunction;
}


}
