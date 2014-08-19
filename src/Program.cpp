#include "Program.h"
#include "IndexList.h"
#include "Function.h"
#include "FunctionInvocation.h"
#include "Scrap.h"

namespace scrap {

Program::Program()
	:	_mainFunction(NULL)
{ }

Program::~Program()
{

}


void Program::Execute()
{
	THROW(NotImplementedException, "HOLD YOUR HORSES; NOT THERE YET.");

	/*
	FunctionInvocation main(_mainFunction, NULL);
	main.Execute();
	*/
}

}
