#include "gtest/gtest.h"
#include "../Method.h"
#include "../Class.h"

using namespace scrap;

// Global MethodAttributes, because they are annoying to create
// every single test.
MethodAttributes attr(ReturnType{VOID, NULL}, 1, Argument{INT, NULL});
Class clss(0, "TestClass");


TEST (MethodTest, TestInvalidConstructorArguments)
{
	// Passing either a 0 as body length or a null pointer should throw 
	// exceptions.
	MethodBody body;

	
	body.code = NULL;
	body.length = 10;
	ASSERT_THROW(Method m(METHOD_NORMAL, &clss, &body, attr);, 
				 NullPointerException);

	body.code = new byte[10];
	body.length = 0;
	ASSERT_THROW(Method m(METHOD_NORMAL, &clss, &body, attr);, 
				 InvalidArgumentException);
	
	delete[] body.code;
}

TEST (MethodTest, TestMethodBodyCreation)
{
	// The method body is copied and placed in a new buffer. The contents
	// must however be identical.
	MethodBody body;
	body.length = 10;
	body.code = new byte[body.length];
	Method m(METHOD_NORMAL, &clss, &body, attr);

	const MethodBody *b= m.GetMethodBody();
	
	ASSERT_EQ(b->length, body.length);

	for (int i=0; i<body.length; i++) {
		ASSERT_EQ(b->code[i], body.code[i]);
	}
	
	delete[] body.code;
}

TEST (MethodTest, TestAttributesAndReturnType)
{
	MethodBody body;
	body.length = 10;
	body.code = new byte[10];

	Method m(METHOD_NORMAL, &clss, &body, attr);

	ASSERT_EQ(m.GetReturnType().type, attr.GetReturnType().type);
	ASSERT_EQ(m.GetMethodAttributes().GetArguments().size(), 
			  attr.GetArguments().size());

	for (int i=0; i<attr.GetArguments().size(); i++) {
		Argument a = attr.GetArguments()[i];
		Argument b = m.GetMethodAttributes().GetArguments()[i];

		ASSERT_EQ(a.type, b.type);
	}

	delete[] body.code;
}

