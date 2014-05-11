#include "Method.h"
#include "Object.h"
#include "Class.h"

#include <cstring>
#include <cstdarg>


namespace scrap {

Method::Method(MethodType type, Class *c, MethodBody *body, MethodAttributes attr)
	:	_type(type),
		_class(c),
		_attrs(attr)
{
	if (body->code == NULL) 
		THROW(NullPointerException, "Method body cannot be NULL");
	if (body->length == 0)
		THROW(InvalidArgumentException, "Method body cannot be of length 0");

	// Copy the original code array into a new buffer
	_body.length = body->length;
	_body.code = new byte[_body.length];
	memcpy(_body.code, body->code, _body.length);
}

Method::~Method()
{
	if (_body.code != NULL) 
		delete[] _body.code;
}


const MethodBody* Method::GetMethodBody() const
{
	return &_body;
}

const Class* Method::GetClass() const 
{
	return _class;
}

MethodType Method::GetMethodType() const 
{
	return _type;
}

const MethodAttributes& Method::GetMethodAttributes() const 
{
	return _attrs;
}

const ReturnType Method::GetReturnType() const 
{
	return _attrs.GetReturnType();
}




MethodAttributes::MethodAttributes(ReturnType ret, vector<Argument> args)
	:	_rettype(ret),
		_args(args)
{

}

MethodAttributes::MethodAttributes(ReturnType ret, int args, ...)
	:	_rettype(ret)
{
	va_list vl;	
	va_start(vl, args);

	for (int i=0; i<args; i++) {
		Argument val = va_arg(vl, Argument);
		_args.push_back(val);
	}

	va_end(vl);
}

ReturnType MethodAttributes::GetReturnType() const
{
	return _rettype;
}

vector<Argument> MethodAttributes::GetArguments() const
{
	return _args;
}

}
