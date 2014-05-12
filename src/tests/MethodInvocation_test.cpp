#include "ScrapTest.h"

Class class0(0, "Class0");
Class class1(1, "Class1");

TEST (MethodInvocationTest, TestValidConstructors)
{
	Method methodNormal = CreateMethod(&class0, METHOD_NORMAL);
	Method methodStatic = CreateMethod(&class0, METHOD_STATIC);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	ASSERT_NO_THROW(MethodInvocation(&methodNormal, obj, NULL));
	ASSERT_NO_THROW(MethodInvocation(&methodStatic, &class0, NULL));
	
	heap.KillOrphans();
}

TEST (MethodInvocationTest, TestInvalidConstructors)
{
	Method methodNormal = CreateMethod(&class0, METHOD_NORMAL);
	Method methodStatic = CreateMethod(&class0, METHOD_STATIC);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	// Call a static method on an object
	ASSERT_ANY_THROW(MethodInvocation(&methodStatic, obj, NULL));

	// Call a normal method on a class
	ASSERT_ANY_THROW(MethodInvocation(&methodNormal, &class0, NULL));

	// Call Class0::mthd on Class1
	ASSERT_ANY_THROW(MethodInvocation(&methodStatic, &class1, NULL));

	heap.KillOrphans();
}

TEST (MethodInvocationTest, TestArgumentAndReturnTransfer)
{
	Method methodA = CreateMethod(&class0, METHOD_NORMAL);
	Method methodB = CreateMethod(&class0, METHOD_NORMAL);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	MethodInvocation invA(&methodA, obj, NULL);
	MethodInvocation invB(&methodB, obj, &invA);

	Stack *stackA = invA.GetStack();
	Stack *stackB = invB.GetStack();

	ASSERT_EQ(stackA->Count(), 0);
	ASSERT_EQ(stackB->Count(), 0);

	// Push the argument to method B
	Variable *var = new Variable(VarType::DOUBLE);
	stackA->Push(var);

	ASSERT_EQ(stackA->Count(), 1);
	ASSERT_NO_THROW(invB.TransferArguments(););
	
	// B should now contain the argument
	ASSERT_EQ(stackA->Count(), 0);
	ASSERT_EQ(stackB->Count(), 1);
	
	// Pop it
	ASSERT_EQ(stackB->Pop(), var);
	ASSERT_EQ(stackB->Count(), 0);
	delete var;

	// Push a return value, push it onto A's stack
	var = new Variable(VarType::INT);
	stackB->Push(var);
	ASSERT_NO_THROW(invB.ReturnValue(););
	
	// The return value should now be contained in stack A
	ASSERT_EQ(stackB->Count(), 0);
	ASSERT_EQ(stackA->Count(), 1);
	ASSERT_EQ(stackA->Pop(), var);

	delete var;
	heap.KillOrphans();
}
