#include "MethodInvocation.h"
#include "Method.h"
#include "Object.h"
#include "Class.h"
#include "Stack.h"
#include "Heap.h"
#include "Variable.h"
#include "Bytecode.h"
#include "IndexList.h"


namespace scrap {

MethodInvocation::MethodInvocation(Heap *heap, Method *method, Object *object, 
									MethodInvocation *caller)
	:	_object(object),
		_class(object->GetClass()),
		_classList(NULL),
		_method(method),
		_caller(caller),
		_pc(0),
		_return(false),
		_heap(heap),
		_executor(this, &_stack, _heap)
{
	if (!_heap || !_method || !_object) {
		THROW(NullPointerException, "NULL-argument to MethodInvocation()");
	}

	if (_method->GetMethodType() == METHOD_STATIC) {
		THROW(InvalidOperationException, "Cannot call static methods on objects");
	}

	// TODO
	// Consider superclasses
	if (_method->GetClass()->GetClassID() != object->GetClass()->GetClassID()) {
		THROW(InvalidClassException, "Method called on object of wrong class");
	}

	TransferArguments();
}

MethodInvocation::MethodInvocation(Heap *heap, Method *method, const Class *c,
									MethodInvocation *caller)
	:	_object(NULL),
		_class(c),
		_classList(NULL),
		_method(method),
		_caller(caller),
		_pc(0),
		_return(false),
		_heap(heap),
		_executor(this, &_stack, _heap)
{
	if (!_heap || !_method || !_class) {
		THROW(NullPointerException, "NULL-argument to MethodInvocation()");
	}

	if (_method->GetMethodType() != METHOD_STATIC) {
		THROW(InvalidOperationException, "Cannot call non-static method on class");
	}
	
	// TODO
	// Consider superclasses
	if (_method->GetClass()->GetClassID() != c->GetClassID()) {
		THROW(InvalidClassException, "Static method called on wrong class");
	}

	TransferArguments();
}

MethodInvocation::~MethodInvocation()
{
	
}


void MethodInvocation::SetClassList(ClassList *clist)
{
	if (clist)
		_classList = clist;
}


void MethodInvocation::Execute()
{
#ifndef _SCRAP_TEST_
	// The ClassList must be defined in non-testing environments
	if (!_classList) {
		THROW(NullPointerException, "The ClassList must be defined in the "
			"executing MethodInvocation instance.");
	}
#endif

	_pc = 0;
	const MethodBody *body = _method->GetMethodBody();

	while (_pc < body->length && !_return) {
		_pc += _executor.Execute(body->code + _pc);
	}
}


void MethodInvocation::PerformMethodCall(Object *object, Method *method)
{
	MethodInvocation invocation(_heap, method, object, this);
	invocation.Execute();
}

void MethodInvocation::PerformMethodCall(Class *c, Method *method)
{
	MethodInvocation invocation(_heap, method, c, this);
	invocation.Execute();
}

void MethodInvocation::BranchToInstruction(unsigned index)
{
	if (index >= _method->GetMethodBody()->length) {
		THROW(IndexOutOfRangeException, "Attempted to branch to undefined index");
	}

	_pc = index;
}

Object* MethodInvocation::InstantiateObject(unsigned classID)
{
#ifdef _SCRAP_TEST_
	// The ClassList may be undefined in testing environments
	if (!_classList) {
		THROW(NullPointerException, 
			"Cannot instantiate objects without a ClassList");
	}
#endif

	Class *c = _classList->GetClass(classID);
	Object *object = _heap->CreateObject(c);
	return object;
}

void MethodInvocation::ReturnToCaller()
{
	ReturnValue();
	_return = true;
}


#ifdef _SCRAP_TEST_
Stack* MethodInvocation::GetStack() 
#else
const Stack* MethodInvocation::GetStack() const 
#endif
{
	return &_stack;
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


void MethodInvocation::TransferArguments()
{
	if (_object) {
		Variable *var = new Variable(VarType::OBJECT);
		var->Set(_object);
		_stack.Push(var);
	}

	if (!_caller) {
		// If arguments are expected, this invocation is invalid
		if (_method->GetMethodAttributes().GetArguments().size() != 0) {
			// TransferArguments() is only called from the constructor, so we can
			// guarantee at this point that the stack holds nothing more than the
			// "this" pointer (it can also be empty for static invocations).
			if (_stack.Count() == 1) 
				delete _stack.Pop();

			THROW(InvalidArgumentException, "Expected parameters, none given");
		}

		return;
	}

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
}
