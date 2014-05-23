#include "ScrapTest.h"

Class class0(0, "Class0");
Class class1(1, "Class1");

TEST (MethodInvocationTest, TestValidConstructors)
{
	Method *methodNormal = CreateVoidMethod(&class0, METHOD_NORMAL);
	Method *methodStatic = CreateVoidMethod(&class0, METHOD_STATIC);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();


	MethodInvocation *inv = NULL;

	// The "this" argument must be popped and deleted
	ASSERT_NO_THROW(inv = new MethodInvocation(methodNormal, obj, NULL));
	delete inv->GetStack()->Pop();
	delete inv;
	
	// No implicit arguments on static methods
	ASSERT_NO_THROW(inv = new MethodInvocation(methodStatic, &class0, NULL));
	delete inv;
	
	delete methodNormal;
	delete methodStatic;
	heap.KillOrphans();
}

TEST (MethodInvocationTest, TestInvalidConstructors)
{
	Method *methodNormal = CreateMethod(&class0, METHOD_NORMAL);
	Method *methodStatic = CreateMethod(&class0, METHOD_STATIC);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	// Call a static method on an object
	ASSERT_ANY_THROW(MethodInvocation(methodStatic, obj, NULL));

	// Call a normal method on a class
	ASSERT_ANY_THROW(MethodInvocation(methodNormal, &class0, NULL));

	// Call Class0::mthd on Class1
	ASSERT_ANY_THROW(MethodInvocation(methodStatic, &class1, NULL));

	heap.KillOrphans();

	delete methodNormal;
	delete methodStatic;
}

TEST (MethodInvocationTest, AssertArgumentRequirement)
{
	Method *method = CreateMethod(&class0, METHOD_NORMAL);
	
	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	// The invocation has no caller and is thus unable to get it's
	// DOUBLE-argument from anywhere. 
	MethodInvocation *inv = NULL;
	ASSERT_ANY_THROW(inv = new MethodInvocation(method, obj, NULL));

	delete method;
	heap.KillOrphans();
}

TEST (MethodInvocationTest, TestArgumentAndReturnTransfer)
{
	Method *methodA = CreateVoidMethod(&class0, METHOD_NORMAL);
	Method *methodB = CreateMethod(&class0, METHOD_NORMAL);

	Heap heap;
	Object *obj = heap.CreateObject(&class0);
	obj->Release();

	// The "this" argument should be pushed onto the stack
	MethodInvocation invA(methodA, obj, NULL);
	Stack *stackA = invA.GetStack();
	ASSERT_EQ(stackA->Count(), 1);

	// Push the argument to method B
	Variable *var = new Variable(VarType::DOUBLE);
	stackA->Push(var);

	MethodInvocation invB(methodB, obj, &invA);
	Stack *stackB = invB.GetStack();
	ASSERT_EQ(stackB->Count(), 2);
	
	// Pop the argument from B
	ASSERT_EQ(stackB->Pop(), var);
	ASSERT_EQ(stackB->Count(), 1);
	delete var;

	// Push a return value, push it onto A's stack
	var = new Variable(VarType::INT);
	stackB->Push(var);
	ASSERT_NO_THROW(invB.ReturnValue(););
	
	// The return value should now be contained in stack A
	ASSERT_EQ(stackB->Count(), 1);
	ASSERT_EQ(stackA->Count(), 2);
	ASSERT_EQ(stackA->Pop(), var);

	// Delete the "this" argument
	delete stackA->Pop();
	delete stackB->Pop();

	delete var;
	delete methodA;
	delete methodB;
	heap.KillOrphans();
}

