#pragma once

#include "Scrap.h"
#include "Stack.h"

namespace scrap {

class Method;
class Object;
class Class;
class Stack;


/* Method Invocation
 * Object that manages execution of a method.
 */
class MethodInvocation {
public:
	MethodInvocation(Method *method, Object *object, MethodInvocation *caller);
	MethodInvocation(Method *method, const Class *c, MethodInvocation *caller);
	~MethodInvocation();

	void Execute();

#ifdef _SCRAP_TEST_		
	// Allow modification in test environments
	Stack* GetStack();	
#else
	const Stack* GetStack() const;
#endif

	// Push return value (top stack value) onto the calling stack
	void ReturnValue();

private:
	const Class *_class;
	Object *_object;
	Method *_method;
	MethodInvocation *_caller;

	Stack _stack;

	// Pop the required arguments from the caller stack and push them to own stack
	void TransferArguments();

};

}
