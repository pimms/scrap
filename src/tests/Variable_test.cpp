#include "ScrapTest.h"

/* VariableTest
 *
 * This test focuses on casting Variable-objects between different types
 * and performing arithmetic operations on them. The tests thoroughly 
 * checks all permutations of casting between compatible types. 
 *
 * Correctly functioning variables is at the heart of the language, and
 * everything must be verified to work or scream loudly when it doesn´t.
 */

TEST (VariableTest, AvailableCasts)
{
	// OBJECT can be casted to OBJECT and BOOL
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::OBJECT), 	true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::INT), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::FLOAT), 	false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::DOUBLE), 	false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::LONG), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::CHAR), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::OBJECT, VarType::BOOL), 		true);
 
	// Int can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::OBJECT), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::INT), 			true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::FLOAT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::DOUBLE), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::INT, VarType::BOOL), 		true);

	// Float can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::OBJECT), 	false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::INT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::FLOAT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::DOUBLE), 	true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::FLOAT, VarType::BOOL), 		true);

	// Double can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::OBJECT), 	false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::INT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::FLOAT), 	true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::DOUBLE), 	true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::DOUBLE, VarType::BOOL), 		true);
	
	// Long can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::OBJECT), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::INT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::FLOAT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::DOUBLE), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::LONG, VarType::BOOL), 		true);

	// Char can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::OBJECT), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::INT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::FLOAT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::DOUBLE), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::CHAR, VarType::BOOL), 		true);

	// Bool can be casted to all bar OBJECT
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::OBJECT), 		false);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::INT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::FLOAT), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::DOUBLE), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::LONG), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::CHAR), 		true);
	EXPECT_EQ(VarValue::CastAvailable(VarType::BOOL, VarType::BOOL), 		true);
}

TEST (VariableTest, CastFromObject)
{
	Variable var(VarType::OBJECT);
	
	var.Set((Object*)NULL);
	EXPECT_EQ(var.Cast(VarType::OBJECT), true);
	EXPECT_EQ(var.Value_a(), (Object*)NULL);
	EXPECT_EQ(var.Type(), VarType::OBJECT);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((Object*)0xDEADBEEF);
	EXPECT_EQ(var.Cast(VarType::OBJECT), true);
	EXPECT_EQ(var.Value_a(), (Object*)0xDEADBEEF);
	EXPECT_EQ(var.Type(), VarType::OBJECT);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((Object*)0xDEADBEEF);
	EXPECT_EQ(var.Cast(VarType::INT), false);
	EXPECT_EQ(var.Cast(VarType::FLOAT), false);
	EXPECT_EQ(var.Cast(VarType::DOUBLE), false);
	EXPECT_EQ(var.Cast(VarType::LONG), false);
	EXPECT_EQ(var.Cast(VarType::CHAR), false);
}

TEST (VariableTest, CastFromInt) 
{
	Variable var(VarType::INT);

	var.Set((int) 4);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)4);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)4);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)4);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)4);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)4);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((int) 4);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastFromFloat) 
{
	Variable var(VarType::FLOAT);

	var.Set((float) 4.f);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)4);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)4);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)4);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)4);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)4);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((float) 4.f);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastFromDouble) 
{
	Variable var(VarType::DOUBLE);

	var.Set((double) 4.0);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)4);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)4);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)4);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)4);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)4);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((double) 4.0);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastFromLong) 
{
	Variable var(VarType::LONG);

	var.Set((long) 4);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)4);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)4);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)4);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)4);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)4);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((long) 4);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastFromChar) 
{
	Variable var(VarType::CHAR);

	var.Set((char) 4);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)4);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)4);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)4);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)4);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)4);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((char) 4);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastFromBool) 
{
	Variable var(VarType::BOOL);

	var.Set((bool) true);

	EXPECT_EQ(var.Cast(VarType::INT), true);
	EXPECT_EQ(var.Value_i(), (int)1);
	EXPECT_EQ(var.Type(), VarType::INT);

	EXPECT_EQ(var.Cast(VarType::FLOAT), true);
	EXPECT_EQ(var.Value_f(), (float)1);
	EXPECT_EQ(var.Type(), VarType::FLOAT);

	EXPECT_EQ(var.Cast(VarType::DOUBLE), true);
	EXPECT_EQ(var.Value_d(), (double)1);
	EXPECT_EQ(var.Type(), VarType::DOUBLE);

	EXPECT_EQ(var.Cast(VarType::LONG), true);
	EXPECT_EQ(var.Value_l(), (long)1);
	EXPECT_EQ(var.Type(), VarType::LONG);

	EXPECT_EQ(var.Cast(VarType::CHAR), true);
	EXPECT_EQ(var.Value_c(), (char)1);
	EXPECT_EQ(var.Type(), VarType::CHAR);

	EXPECT_EQ(var.Cast(VarType::BOOL), true);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Cast(VarType::OBJECT), false);
}

TEST (VariableTest, CastToBool)
{
	Variable var;

	// Object
	var.Set((Object*) NULL);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((Object*) 0xDEADBEEF);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	// int
	var.Set((int) 5);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((int) 0);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	// float
	var.Set((float) 5.f);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((float) 0.f);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	// double
	var.Set((double) 5.0);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((double) 0.0);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	// long
	var.Set((long) 5);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((long) 0);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	// char
	var.Set((char) 5);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), true);
	EXPECT_EQ(var.Type(), VarType::BOOL);

	var.Set((char) 0);
	var.Cast(VarType::BOOL);
	EXPECT_EQ(var.Value_b(), false);
	EXPECT_EQ(var.Type(), VarType::BOOL);
}


/* Arithmetic Tests
 *
 * Ensure that all operations give the correct result and
 * that operations on different types / invalid operations on
 * types are prohibited with exceptions.
 */


// Concatenate "Value_" and "_T" to "Value_`_T`()". For _T=i,
// the result is "Value_i()".
#define TM_VALUE_T(_T) 	Value_ ## _T()

// Define two Variables, calculate the result, assert equality
// with regular C-variables. 
//
// Example values for parameters:
// 		_OPFUNC 			Add		Variable::_OPFUNC
// 		_OP 				+ 		Must be a valid operand	
// 		_T 					i		Shorthand type mnemonic
// 		_TYPE 				int 	"Real-world" type
#define TM_ARIT_GENERIC(_OPFUNC, _OP, _T, _TYPE) 	\
	{												\
		_TYPE va = (_TYPE)4.5;						\
		_TYPE vb = (_TYPE)9.9;						\
		_TYPE r = va _OP vb;						\
		Variable a(VarType::_T);					\
		Variable b(VarType::_T);					\
		a.Set((_TYPE) va);							\
		b.Set((_TYPE) vb);							\
		a._OPFUNC(&b);								\
		ASSERT_EQ(a.TM_VALUE_T(_T), r);				\
	}


#define TM_ADD(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Add, +, _T, _TYPE)
#define TM_SUB(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Sub, -, _T, _TYPE)
#define TM_MUL(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Mul, *, _T, _TYPE)
#define TM_DIV(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Div, /, _T, _TYPE) 
#define TM_SHR(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Shr, >>, _T, _TYPE) 
#define TM_SHL(_T, _TYPE)							\
	TM_ARIT_GENERIC(Shl, <<, _T, _TYPE)
#define TM_MOD(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Mod, %, _T, _TYPE)
#define TM_XOR(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Xor, ^, _T, _TYPE)
#define TM_AND(_T, _TYPE) 							\
	TM_ARIT_GENERIC(And, &, _T, _TYPE)
#define TM_OR(_T, _TYPE) 							\
	TM_ARIT_GENERIC(Or, |, _T, _TYPE)

// Assert that an operation is invalid. 
#define TM_INVALID_OP(_T, _OPFUNC) 					\
	{												\
		Variable a(VarType::_T);					\
		Variable b(VarType::_T);					\
		ASSERT_ANY_THROW(a._OPFUNC(&b));				\
	}

TEST (VariableTest, TestAdd)
{
	TM_ADD(i, int);
	TM_ADD(f, float);
	TM_ADD(d, double);
	TM_ADD(l, long);
	TM_ADD(c, char);

	TM_INVALID_OP(a, Add);
}

TEST (VariableTest, TestSub)
{
	TM_SUB(i, int);
	TM_SUB(f, float);
	TM_SUB(d, double);
	TM_SUB(l, long);
	TM_SUB(c, char);

	TM_INVALID_OP(a, Sub);
}

TEST (VariableTest, TestMul)
{
	TM_MUL(i, int);
	TM_MUL(f, float);
	TM_MUL(d, double);
	TM_MUL(l, long);
	TM_MUL(c, char);

	TM_INVALID_OP(a, Mul);
}

TEST (VariableTest, TestDiv)
{
	TM_DIV(i, int);
	TM_DIV(f, float);
	TM_DIV(d, double);
	TM_DIV(l, long);
	TM_DIV(c, char);

	TM_INVALID_OP(a, Div);
}

TEST (VariableTest, TestShr)
{
	TM_SHR(i, int);
	TM_SHR(l, long);
	TM_SHR(c, char);

	TM_INVALID_OP(f, Shr);
	TM_INVALID_OP(d, Shr);
	TM_INVALID_OP(a, Shr);
}

TEST (VariableTest, TestShl)
{
	TM_SHL(i, int);
	TM_SHL(l, long);
	TM_SHL(c, char);

	TM_INVALID_OP(f, Shl);
	TM_INVALID_OP(d, Shl);
	TM_INVALID_OP(a, Shl);
}

TEST (VariableTest, TestMod)
{
	TM_MOD(i, int);
	TM_MOD(l, long);
	TM_MOD(c, char);

	TM_INVALID_OP(f, Mod);
	TM_INVALID_OP(d, Mod);
	TM_INVALID_OP(a, Mod);
}

TEST (VariableTest, TestXor)
{
	TM_XOR(i, int);
	TM_XOR(l, long);
	TM_XOR(c, char);

	TM_INVALID_OP(f, Xor);
	TM_INVALID_OP(d, Xor);
	TM_INVALID_OP(a, Xor);
}

TEST (VariableTest, TestAnd)
{
	TM_AND(i, int);
	TM_AND(l, long);
	TM_AND(c, char);

	TM_INVALID_OP(f, And);
	TM_INVALID_OP(d, And);
	TM_INVALID_OP(a, And);
}

TEST (VariableTest, TestOr)
{
	TM_OR(i, int);
	TM_OR(l, long);
	TM_OR(c, char);

	TM_INVALID_OP(f, Or);
	TM_INVALID_OP(d, Or);
	TM_INVALID_OP(a, Or);
}
