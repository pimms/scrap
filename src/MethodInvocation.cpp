#include "MethodInvocation.h"
#include "Method.h"
#include "Object.h"
#include "Class.h"
#include "Stack.h"
#include "Heap.h"
#include "Variable.h"
#include "Bytecode.h"


namespace scrap {

MethodInvocation::MethodInvocation(Method *method, Object *object, 
									MethodInvocation *caller)
	:	_object(object),
		_class(object->GetClass()),
		_method(method),
		_caller(caller)
{
	if (_method->GetMethodType() == METHOD_STATIC) {
		THROW(InvalidOperationException, "Cannot call static methods on objects");
	}

	if (_method->GetClass()->GetClassID() != object->GetClass()->GetClassID()) {
		THROW(InvalidClassException, "Method called on object of wrong class");
	}
}

MethodInvocation::MethodInvocation(Method *method, const Class *c,
									MethodInvocation *caller)
	:	_object(NULL),
		_class(c),
		_method(method),
		_caller(caller)
{
	if (_method->GetMethodType() != METHOD_STATIC) {
		THROW(InvalidOperationException, "Cannot call non-static method on class");
	}
	
	if (_method->GetClass()->GetClassID() != c->GetClassID()) {
		THROW(InvalidClassException, "Static method called on wrong class");
	}
}

MethodInvocation::~MethodInvocation()
{
	
}


void MethodInvocation::TransferArguments()
{
	if (!_caller)
		return;

	MethodAttributes attr = _method->GetMethodAttributes();

	for (int i=0; i<attr.GetArguments().size(); i++) {
		TypeDesc arg = attr.GetArguments()[i];
		Variable *var = _caller->_stack.Pop();

		// In the future, the variable will be attempted casted to
		// the expected type.
		if (var->Type() != arg.type) {
			string err = "Value of type " + VarTypeToString(var->Type()) +
						 " passed to argument expecting " + 
						 VarTypeToString(arg.type);
			THROW(InvalidArgumentException, err);
		}

		_stack.Push(var);
	}
}

void MethodInvocation::ReturnValue()
{
	if (!_caller)
		return;

	Variable *var = _stack.Pop();

	if (var->Type() != _method->GetReturnType().type) {
		string err = "Attempted to return value of type " 
			+ VarTypeToString(var->Type())
			+ " from method with return type " 
			+ VarTypeToString(_method->GetReturnType().type);
		THROW(InvalidOperationException, err);
	}

	_caller->_stack.Push(var);
}

#ifdef _SCRAP_TEST_
Stack* MethodInvocation::GetStack() 
#else
const Stack* MethodInvocation::GetStack() const 
#endif
{
	return &_stack;
}
	
}