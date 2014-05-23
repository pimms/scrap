#pragma once

#include "Scrap.h"
#include "Stack.h"
#include "Executor.h"


namespace scrap {

class Method;
class Object;
class Class;
class Stack;
class Heap;


/* Method Invocation
 * Object that manages execution of a method. 
 *
 * When calling an instance method, the Object-instance is wrapped in a Variable
 * and pushed FIRST onto the stack as the "this" parameter.
 */
class MethodInvocation : public ExecutionDelegate {
public:
	MethodInvocation(Heap *heap, Method *method, Object *object, 
					 MethodInvocation *caller);
	MethodInvocation(Heap *heap, Method *method, const Class *c, 
					 MethodInvocation *caller);
	~MethodInvocation();

	void Execute();

	void PerformMethodCall(Object *object, Method *method);
	void PerformMethodCall(Class *c, Method *method);
	void ReturnToCaller();

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
	Heap *_heap;
	Executor _executor;

	unsigned _pc;

	// Pop the required arguments from the caller stack and push them to own stack
	void TransferArguments();
};

}
