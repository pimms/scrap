#include "FunctionInvocation.h"
#include "Function.h"
#include "Stack.h"
#include "Variable.h"
#include "Bytecode.h"
#include "IndexList.h"
#include "Debug.h"


namespace scrap {

FunctionInvocation::FunctionInvocation(Function *function, FunctionInvocation *caller)
	:	_function(function),
		_caller(caller),
		_pc(0),
		_return(false),
		_executor(this, &_stack),
		_debugger(NULL)
{
	if (!function) {
		THROW(NullPointerException, "NULL-pointer given as arg to FunctionInvocation()");
	}

	TransferArguments();
}

FunctionInvocation::~FunctionInvocation()
{
	
}



void FunctionInvocation::Execute()
{
	_pc = 0;
	const FunctionBody *body = _function->GetFunctionBody();

	if (_debugger) {
		_debugger->DidInvokeNewFunction(this, _caller);
	}

	while (_pc < body->length && !_return) {
		if (_debugger) {
			_debugger->WillExecuteInstruction(this, body->code + _pc);
		}

		_pc += _executor.Execute(body->code + _pc);
	}

	if (!_return && _debugger) {
		/* No T_RETURN was executed, the method ran fully to completion.
		 * The Debugger has therefore not been notified of the returnal of this method.
		 */
		_debugger->DidReturn(this, NULL);
	}
}


void FunctionInvocation::PerformFunctionCall(Function *function)
{
	FunctionInvocation invocation(function, this);

	if (_debugger)
		invocation.SetDebugger(_debugger);

	invocation.Execute();
}

void FunctionInvocation::BranchToInstruction(unsigned index)
{
	if (index >= _function->GetFunctionBody()->length) {
		THROW(IndexOutOfRangeException, "Attempted to branch to undefined index");
	}

	_pc = index;
}

void FunctionInvocation::ReturnToCaller()
{
	ReturnValue();
	_return = true;
}


#ifdef _SCRAP_TEST_
Stack* FunctionInvocation::GetMutableStack()
{
	return &_stack;
}
#endif


const Stack* FunctionInvocation::GetStack() const 
{
	return &_stack;
}

const Function* FunctionInvocation::GetFunction() const 
{
	return _function;
}


void FunctionInvocation::ReturnValue()
{
	if (!_caller)
		return;

	Variable *var = _stack.Pop();

	if (var->Type() != _function->GetReturnType().type) {
		string err = "Attempted to return value of type " 
			+ VarTypeToString(var->Type())
			+ " from method with return type " 
			+ VarTypeToString(_function->GetReturnType().type);
		THROW(InvalidOperationException, err);
	}

	_caller->_stack.Push(var);

	if (_debugger) {
		_debugger->DidReturn(this, var);
	}
}


void FunctionInvocation::SetDebugger(Debugger *debugger) {
	_debugger = debugger;
}



void FunctionInvocation::TransferArguments()
{
	if (!_caller) {
		// If arguments are expected, this invocation is invalid
		if (_function->GetFunctionAttributes().GetArguments().size() != 0) {
			// TransferArguments() is only called from the constructor, so we can
			// guarantee at this point that the stack holds nothing more than the
			// "this" pointer (it can also be empty for static invocations).
			if (_stack.Count() == 1) 
				delete _stack.Pop();

			THROW(InvalidArgumentException, "Expected parameters, none given");
		}

		return;
	}

	FunctionAttributes attr = _function->GetFunctionAttributes();

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
