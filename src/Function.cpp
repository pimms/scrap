#include "Function.h"

#include <cstring>
#include <cstdarg>


namespace scrap {

Function::Function(FunctionBody *body, FunctionAttributes attr)
	:	_attrs(attr)
{
	if (body->code == NULL) 
		THROW(NullPointerException, "Function body cannot be NULL");
	if (body->length == 0)
		THROW(InvalidArgumentException, "Function body cannot be of length 0");

	// Copy the original code array into a new buffer
	_body.length = body->length;
	_body.code = new byte[_body.length];
	memcpy(_body.code, body->code, _body.length);
}

Function::~Function()
{
	delete[] _body.code;
}


const FunctionBody* Function::GetFunctionBody() const
{
	return &_body;
}

const FunctionAttributes& Function::GetFunctionAttributes() const 
{
	return _attrs;
}

const TypeDesc Function::GetReturnType() const 
{
	return _attrs.GetReturnType();
}




FunctionAttributes::FunctionAttributes(TypeDesc ret, vector<TypeDesc> args, string name)
	:	_rettype(ret),
		_args(args),
		_name(name)
{

}

TypeDesc FunctionAttributes::GetReturnType() const
{
	return _rettype;
}

vector<TypeDesc> FunctionAttributes::GetArguments() const
{
	return _args;
}

string FunctionAttributes::GetName() const
{
	return _name;
}

}
