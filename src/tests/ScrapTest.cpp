#include "ScrapTest.h"

Function* CreateFunction() 
{
	FunctionBody body;
	body.length = 10;
	body.code = new byte[10];

	vector<TypeDesc> args;
	args.push_back(TypeDesc{DOUBLE});

	FunctionAttributes attr(TypeDesc{INT}, args, "GenericTestFunction");
	Function *m = new Function(&body, attr);

	delete[] body.code;
	return m;
}

Function* CreateVoidFunction() 
{
	FunctionBody body;
	body.length = 10;
	body.code = new byte[10];

	vector<TypeDesc> args;

	FunctionAttributes attr(TypeDesc{VOID}, args, "GenericTestFunctionVoid");
	Function *m = new Function(&body, attr);

	delete[] body.code;
	return m;
}
