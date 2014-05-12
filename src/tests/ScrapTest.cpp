#include "ScrapTest.h"

Method CreateMethod(Class *c, MethodType type) 
{
	MethodBody body;
	body.length = 10;
	body.code = new byte[10];

	MethodAttributes attr(TypeDesc{INT}, 1, TypeDesc{DOUBLE});
	Method m(type, c, &body, attr);

	delete[] body.code;
	return m;
}
