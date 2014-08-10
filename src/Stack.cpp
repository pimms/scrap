#include "Stack.h"
#include "Variable.h"


namespace scrap {

Stack::Stack()
{

}

Stack::~Stack()
{
	while (Count()) 
		delete Pop();
}


void Stack::Push(Variable *var)
{
	if (_deque.size() == MaxStackSize())
		THROW(StackOverflowException, "Stack overflow");
	_deque.push_back(var);
}

Variable* Stack::Pop()
{
	if (_deque.size() == 0)
		THROW(StackUnderflowException, "Stack underflow");
	
	Variable *var = _deque.back();
	_deque.pop_back();
	return var;
}


int Stack::Count() const 
{
	return _deque.size();
}


int Stack::MaxStackSize()
{
	return STACK_MAX_SIZE;
}


#ifdef _SCRAP_TEST_
Stack* Stack::Copy() const
{
	Stack *copy = new Stack;
	copy->_deque = _deque;
	return copy;
}
#endif


const Variable* Stack::Peek(int index) const 
{
	if (index >= 0 && index < Count()) 
		return _deque.at(index);
	return NULL;
}

}
