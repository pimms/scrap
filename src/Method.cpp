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

const TypeDesc Method::GetReturnType() const 
{
	return _attrs.GetReturnType();
}




MethodAttributes::MethodAttributes(TypeDesc ret, vector<TypeDesc> args)
	:	_rettype(ret),
		_args(args)
{

}

TypeDesc MethodAttributes::GetReturnType() const
{
	return _rettype;
}

vector<TypeDesc> MethodAttributes::GetArguments() const
{
	return _args;
}

}
