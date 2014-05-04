#include "Stack.h"
#include "Variable.h"


namespace scrap {

Stack::Stack()
{

}

Stack::~Stack()
{
	// Poor stack management - upon destruction, the stack must be empty.
	if (Count() != 0) {
		THROW(StackNotEmptyException, 
			"Upon stack destruction, the stack must be empty");
	}
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

}
