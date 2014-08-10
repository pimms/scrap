#pragma once

#include "Scrap.h"
#include "Stack.h"
#include "Executor.h"


namespace scrap {

class Method;
class Object;
class Class;
class ClassList;
class Stack;
class Heap;
class Debugger;


/* Method Invocation
 * Object that manages execution of a method. 
 *
 * When calling an instance method, the Object-instance is wrapped in a Variable
 * and pushed FIRST onto the stack as the "this" parameter.
 *
 * MethodInvocations instantiate other MethodInvocations upon ivoking other methods. 
 * To maintain a consistent heap between methods, the Heap-instance is passed down
 * to invoked methods. The Stack-instances are however privately created and are 
 * discarded upon the method's returnal.
 *
 * A Debugger-instance may be passed via the "SetDebugger(Debugger*)" method. 
 */
class MethodInvocation : public ExecutionDelegate {
public:
	MethodInvocation(Heap *heap, Method *method, Object *object, 
					 MethodInvocation *caller);
	MethodInvocation(Heap *heap, Method *method, const Class *c, 
					 MethodInvocation *caller);
	~MethodInvocation();

	/* The ClassList must be defined for the MethodInvocation to be able to
	 * instantiate objects. In test environments, the CL is optional but it
	 * is required in non-test environments.
	 */
	void SetClassList(ClassList *classList);

	void Execute();


	void PerformMethodCall(Object *object, Method *method);
	void PerformMethodCall(Class *c, Method *method);
	void BranchToInstruction(unsigned index);
	Object* InstantiateObject(unsigned classID);
	void ReturnToCaller();

	/* Getter Methods  */
#ifdef _SCRAP_TEST_		
	// Allow modification in test environments
	Stack* GetMutableStack();	
#endif

	const Stack* GetStack() const;
	const Method* GetMethod() const;
	const ClassList* GetClassList() const;


	// Push return value (top stack value) onto the calling stack
	void ReturnValue();

	/* A Debugger-instance may be assigned to the MethodInvocation. The Debugger
	 * will be passed down to any invocations invoked by this invocation.
	 */
	void SetDebugger(Debugger *debugger);


private:
	const Class *_class;
	ClassList *_classList;
	Object *_object;
	Method *_method;
	MethodInvocation *_caller;

	Stack _stack;
	Heap *_heap;
	Executor _executor;

	Debugger *_debugger;


	unsigned _pc;
	bool _return;

	// Pop the required arguments from the caller stack and push them to own stack
	void TransferArguments();
};

}
