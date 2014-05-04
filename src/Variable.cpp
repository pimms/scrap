#include "Variable.h"
#include "Object.h"

#include <cstring>

namespace scrap {


string VarTypeToString(VarType t)
{
	switch (t) {
		case OBJECT:
			return "Object";
		case INT:
			return "int";
		case FLOAT:
			return "float";
		case DOUBLE:
			return "double";
		case LONG:
			return "long";
		case CHAR:
			return "char";
		case BOOL:
			return "bool";
	}
	
	return "Undefined";
}

// Cast a VarValue of type "type" to <TO>. NO CHECKING IS PERFORMED
// TO SEE IF THIS IS A LEGAL CAST, THE CALLER MUST CHECK THIS. 
template <typename TO>
TO CastVarValue(VarValue val, VarType type)
{
	switch (type) {
		case INT:
			return static_cast<TO>(val.i);
		case FLOAT:
			return static_cast<TO>(val.f);
		case DOUBLE:
			return static_cast<TO>(val.d);
		case LONG:
			return static_cast<TO>(val.l);
		case CHAR:
			return static_cast<TO>(val.c);
		case BOOL:
			return static_cast<TO>((bool)val.c);
		case OBJECT:
			THROW(InvalidCastException,
				"Unable to cast to Object* from CastVarValue().");
	
	}
}


bool VarValue::CastAvailable(VarType from, VarType to)
{
	switch (from) {
		// Objects can be casted to other objects (wut) and bool. 
		case OBJECT:
			switch (to) {
				case OBJECT:
				case BOOL:
					return true;
				default:
					return false;
			}

		// Integral types can be freely casted among themselves
		case INT:
		case FLOAT:
		case DOUBLE:
		case LONG:
		case CHAR:
		case BOOL:
			switch (to) {
				case INT:
				case FLOAT:
				case DOUBLE:
				case LONG:
				case CHAR:
				case BOOL:
					return true;

				// Nothing but OBJECT can be casted to OBJECT.
				case OBJECT:
				default:
					return false;
			}
	}

	/* NEVER REACHED */
	return false;
}

VarValue VarValue::CastTo(VarValue val, VarType from, VarType to)
{
	if (!VarValue::CastAvailable(from, to)) {
		THROW(InvalidCastException, 
			"Unable to cast type " + VarTypeToString(from) + 
			" to type " + VarTypeToString(to));
	}

	VarValue ret;

	switch (to) {
		case INT:
			ret.i = CastVarValue<int>(val, from);
			break;
		case FLOAT:
			ret.f = CastVarValue<float>(val, from);
			break;
		case DOUBLE:
			ret.d = CastVarValue<double>(val, from);
			break;
		case LONG:
			ret.l = CastVarValue<long>(val, from);
			break;
		case CHAR:
			ret.c = CastVarValue<char>(val, from);
			break;
		case BOOL:
			// Avoid using CastVarValues(), as it has no way
			// of checking the destination type.
			if (from == VarType::OBJECT) {
				ret.c = (bool)val.a;
			} else {
				ret.c = CastVarValue<bool>(val, from);
			}
			break;
		case OBJECT:
			if (from == VarType::OBJECT) {
				ret.a = val.a;
			} else {
				THROW(InvalidCastException,
					"Impossible cast destination type: " + VarTypeToString(to));
			}
			break;

		default:
			THROW(InvalidCastException,
				"Undefined cast destination type: " + VarTypeToString(to));
	}

	return ret;
}



Variable::Variable(VarType type)
	:	_type(type)
{
	// All variables are initiated with zero in some form. Booleans
	// are false, ints 0 and objects NULL by default.
	memset(&_value, 0, sizeof(VarValue));
}

Variable::Variable()
	: 	Variable(VarType::OBJECT)
{

}

Variable::~Variable()
{
	if (_type == OBJECT && _value.a) {
		// TODO:
		// Decrement counter of object
	}
}


bool Variable::Cast(VarType type)
{
	if (!VarValue::CastAvailable(_type, type))
		return false;

	_value = VarValue::CastTo(_value, _type, type);
	_type = type;

	return true;
}


void Variable::Set(Object *a)
{
	_value.a = a;
	_type = OBJECT;
}

void Variable::Set(int i)
{
	_value.i = i;
	_type = INT;
}

void Variable::Set(float f)
{
	_value.f = f;
	_type = FLOAT;
}

void Variable::Set(double d)
{
	_value.d = d;
	_type = DOUBLE;
}

void Variable::Set(long l)
{
	_value.l = l;
	_type = LONG;
}

void Variable::Set(char c)
{
	_value.c = c;
	_type = CHAR;
}

void Variable::Set(bool c)
{
	_value.c = c;
	_type = BOOL;
}


// Macro for getting the value of the _value-member. If the type
// does not match, throw an exception. The "type" argument must be
// one of the common mnemonics for the types: "a", "i", etc.
#define RETURN_VAL_TYPE(type) 					\
	if (_type == type) 							\
		return _value.type; 					\
	else										\
		THROW(InvalidTypeException, 			\
		(string)"Attempt to return type " 		\
				+ VarTypeToString(type) 		\
				+ " on object of type "  		\
				+ VarTypeToString(_type));

Object* Variable::Value_a() const
{
	RETURN_VAL_TYPE(a);
}

int Variable::Value_i() const 
{
	RETURN_VAL_TYPE(i);
}

float Variable::Value_f() const 
{
	RETURN_VAL_TYPE(f);
}

double Variable::Value_d() const 
{
	RETURN_VAL_TYPE(d);
}

long Variable::Value_l() const 
{
	RETURN_VAL_TYPE(l);
}

char Variable::Value_c() const 
{
	RETURN_VAL_TYPE(c);
}

bool Variable::Value_b() const 
{
	RETURN_VAL_TYPE(b);
}


VarType Variable::Type() const 
{
	return _type;
}


void Variable::Add(const Variable &var) 
{

}

void Variable::Sub(const Variable &var) 
{

}

void Variable::Mul(const Variable &var) 
{

}

void Variable::Div(const Variable &var) 
{

}

void Variable::Shr(unsigned steps)
{

}

void Variable::Shl(unsigned steps)
{

}

void Variable::Mod(const Variable &var)
{

}

void Variable::Xor(const Variable &var)
{

}


}
