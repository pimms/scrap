#include "Variable.h"
#include "Object.h"

#include <cstring>

namespace scrap {


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
		case VOID:
			THROW(InvalidCastException,
				"Unable to cast to 'void' from CastVarValue().");
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

		case VOID:
			return false;
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
	:	_type(type),
		_fieldVar(false)
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

}


Variable* Variable::Copy()
{
	Variable *cpy = new Variable();
	cpy->_type = _type;
	cpy->_value = _value;

	// Field variable status is not copied
	cpy->_fieldVar = false;

	return cpy;
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


// Macro for throwing exception when an operation is unimplemented
// for a given type. This exception is NEVER expected to be cast
// during execution and is only a safety measure in case I forgot it.
#define THROW_UNIMPLEMENTED_OP(_OP) 							\
	THROW(NotImplementedException,								\
		"Operation \"" + AritOpToString(AritOp::ADD) 			\
		+ "\" not implemented for type " 						\
		+ VarTypeToString(_type));a

// Macro for checking if the value _VAR._V of type _T is 0,
// in which case a DivisionByZeroException is thrown.
#define ZERO_DIV_CHECK(_VAR, _V, _T) 							\
	if (_VAR._V == (_T)0) 										\
		THROW(DivisionByZeroException, 							\
			"Attempted divison by zero");
	

void Variable::Add(const Variable *var) 
{
	ValidOperationCheck(AritOp::ADD, var);

	switch (_type) {
		case INT:
			_value.i += var->_value.i;
			break;
		case FLOAT:
			_value.f += var->_value.f;
			break;
		case DOUBLE:
			_value.d += var->_value.d;
			break;
		case LONG:
			_value.l += var->_value.l;
			break;
		case CHAR:
			_value.c += var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::ADD);
	}
}

void Variable::Sub(const Variable *var) 
{
	ValidOperationCheck(AritOp::SUB, var);

	switch (_type) {
		case INT:
			_value.i -= var->_value.i;
			break;
		case FLOAT:
			_value.f -= var->_value.f;
			break;
		case DOUBLE:
			_value.d -= var->_value.d;
			break;
		case LONG:
			_value.l -= var->_value.l;
			break;
		case CHAR:
			_value.c -= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::SUB);
	}
}

void Variable::Mul(const Variable *var) 
{
	ValidOperationCheck(AritOp::MUL, var);

	switch (_type) {
		case INT:
			_value.i *= var->_value.i;
			break;
		case FLOAT:
			_value.f *= var->_value.f;
			break;
		case DOUBLE:
			_value.d *= var->_value.d;
			break;
		case LONG:
			_value.l *= var->_value.l;
			break;
		case CHAR:
			_value.c *= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::MUL);
	}
}

void Variable::Div(const Variable *var) 
{
	ValidOperationCheck(AritOp::DIV, var);

	switch (_type) {
		case INT:
			ZERO_DIV_CHECK(var->_value, i, int);
			_value.i /= var->_value.i;
			break;
		case FLOAT:
			ZERO_DIV_CHECK(var->_value, f, float);
			_value.f /= var->_value.f;
			break;
		case DOUBLE:
			ZERO_DIV_CHECK(var->_value, d, double);
			_value.d /= var->_value.d;
			break;
		case LONG:
			ZERO_DIV_CHECK(var->_value, l, long);
			_value.l /= var->_value.l;
			break;
		case CHAR:
			ZERO_DIV_CHECK(var->_value, c, char);
			_value.c /= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::DIV);
	}
}

void Variable::Shr(const Variable *var)
{
	ValidOperationCheck(AritOp::SHR, var);

	switch (_type) {
		case INT:
			_value.i >>= var->_value.i;
			break;
		case LONG:
			_value.l >>= var->_value.l;
			break;
		case CHAR:
			_value.c >>= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::SHR);
	}
}

void Variable::Shl(const Variable *var)
{
	ValidOperationCheck(AritOp::SHL, var);

	switch (_type) {
		case INT:
			_value.i <<= var->_value.i;
			break;
		case LONG:
			_value.l <<= var->_value.l;
			break;
		case CHAR:
			_value.c <<= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::SHL);
	}
}

void Variable::Mod(const Variable *var)
{
	ValidOperationCheck(AritOp::MOD, var);

	switch (_type) {
		case INT:
			ZERO_DIV_CHECK(var->_value, i, int);
			_value.i %= var->_value.i;
			break;
		case LONG:
			ZERO_DIV_CHECK(var->_value, l, long);
			_value.l %= var->_value.l;
			break;
		case CHAR:
			ZERO_DIV_CHECK(var->_value, c, char);
			_value.c %= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::MOD);
	}
}

void Variable::Xor(const Variable *var)
{
	ValidOperationCheck(AritOp::XOR, var);

	switch (_type) {
		case INT:
			_value.i ^= var->_value.i;
			break;
		case LONG:
			_value.l ^= var->_value.l;
			break;
		case CHAR:
			_value.c ^= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::XOR);
	}
}

void Variable::And(const Variable *var)
{
	ValidOperationCheck(AritOp::XOR, var);

	switch (_type) {
		case INT:
			_value.i &= var->_value.i;
			break;
		case LONG:
			_value.l &= var->_value.l;
			break;
		case CHAR:
			_value.c &= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::AND);
	}
}

void Variable::Or(const Variable *var)
{
	ValidOperationCheck(AritOp::XOR, var);

	switch (_type) {
		case INT:
			_value.i |= var->_value.i;
			break;
		case LONG:
			_value.l |= var->_value.l;
			break;
		case CHAR:
			_value.c |= var->_value.c;
			break;
		default:
			THROW_UNIMPLEMENTED_OP(AritOp::OR);
	}
}


void Variable::SetFieldVariableFlag(bool fieldFlag)
{
	_fieldVar = fieldFlag;
}

bool Variable::IsFieldVariable() const
{
	return _fieldVar;
}



void Variable::ValidOperationCheck(AritOp op, const Variable *var)
{
	if (_type != var->Type() || _type == VarType::OBJECT) {
		THROW(InvalidTypeException, 
			"Cannot perform operation " + AritOpToString(op) +
			" with left operand of type " + VarTypeToString(_type) +
			" and right operand of type " + VarTypeToString(var->Type()));
	}

	if (!IsOperationAvailable(op, var->Type())) {
		THROW(InvalidOperationException, 
			"Cannot perform operation " + AritOpToString(op) +
			" with left operand of type " + VarTypeToString(_type) +
			" and right operand of type " + VarTypeToString(var->Type()));
	}
}


bool Variable::IsOperationAvailable(AritOp op, VarType type)
{
	switch (type) {
		// All operations can be performed on integral primitives
		case INT:
		case LONG:
		case CHAR:
			return true;

		// No operations can be performed on bool, void or objects
		case BOOL:
		case OBJECT:
		case VOID:
			return false;

		case FLOAT:
		case DOUBLE:
			switch (op) {
				case ADD:
				case SUB:
				case MUL:
				case DIV:
					return true;
				default:
					return false;
			}
	}
}

}
