#include "ScrapTest.h"


TEST (StackTest, TestPushPop)
{
	const int max = 20;
	Stack stack;

	for (int i=0; i<max; i++) {
		Variable *v = new Variable(scrap::VarType::INT);
		stack.Push(v);
		ASSERT_EQ(stack.Count(), i+1);
	}

	for (int i=max-1; i>=0; i--) {
		Variable *v = stack.Pop();
		ASSERT_EQ(stack.Count(), i);
		delete v;
	}
}

TEST (StackTest, TestOverflow)
{
	ASSERT_TRUE(Stack::MaxStackSize() > 0);

	Stack stack;

	for (int i=0; i<Stack::MaxStackSize(); i++) {
		Variable *var = new Variable(scrap::VarType::INT);
		stack.Push(var);
	}

	// Pushing one more will cast an exception, and the object will 
	// not actually be added to the stack.
	Variable *var = new Variable(scrap::VarType::INT);
	ASSERT_THROW(stack.Push(var), scrap::StackOverflowException);
	ASSERT_EQ(stack.Count(), Stack::MaxStackSize());
	delete var;

	for (int i=0; i<Stack::MaxStackSize(); i++) 
		delete stack.Pop();

	ASSERT_EQ(stack.Count(), 0);
}

TEST (StackTest, TestUnderflow)
{
	Stack stack;
	ASSERT_THROW(stack.Pop(), scrap::StackUnderflowException);
}


