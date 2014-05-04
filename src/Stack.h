#pragma once

#include "Scrap.h"
#include <deque>

using std::deque;

#define STACK_MAX_SIZE 1024

namespace scrap {

class Variable;

/* Stack
 * Each method invocation creates a new stack. The stack manages local variables
 * and literals. If the stack contains a non-zero amount of elements upon
 * destruction, a StackNotEmptyException is thrown. 
 */
class Stack {
public:
	Stack();
	~Stack();

	// Push a variable onto the stack
	void Push(Variable *var);

	// Pop a variable from the stack
	Variable* Pop();

	int Count() const;

	// TODO: Consider setting the stack dynamically
	// Returns STACK_MAX_SIZE for now.
	static int MaxStackSize();

private:
	// All Variables managed by a stack are deleted by the stack
	// upon destruction. 
	deque<Variable*> _deque;
};

}
