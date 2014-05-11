#pragma once

#include "Scrap.h"


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
	MethodInvocation(Method *method, Object *object);
	MethodInvocation(Method *method, Class *c);
	~MethodInvocation();

	// Pop the required arguments from the caller stack and push them
	// onto this stack.
	void TransferArguments(MethodInvocation *caller);

private:
	

};

}
