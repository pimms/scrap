#include "ScrapTest.h"

Method CreateMethod(Class *c, MethodType type) 
{
	MethodBody body;
	body.length = 10;
	body.code = new byte[10];

	vector<TypeDesc> args;
	args.push_back(TypeDesc{DOUBLE});

	MethodAttributes attr(TypeDesc{INT}, args, "GenericTestMethod");
	Method m(type, c, &body, attr);

	delete[] body.code;
	return m;
}
