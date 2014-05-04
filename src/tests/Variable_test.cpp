#include "../Variable.h"
#include "../Object.h"
#include "../Scrap.h"
#include "gtest/gtest.h"

using scrap::Variable;
using scrap::VarType;
using scrap::VarValue;
using scrap::Object;

/* VariableTest
 *
 * This test focuses on casting Variable-objects between different types
 * and performing arithmetic operations on them. The tests thoroughly 
 * checks all permutations of casting between compatible types. 
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
