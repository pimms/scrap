#pragma once

#include "Scrap.h"
#include "Stack.h"
#include "Executor.h"


namespace scrap {

class Function;
class Stack;
class Debugger;


/* Function Invocation
 * Object that manages execution of a function. 
 *
 * FunctionInvocations instantiate other FunctionInvocations upon 
 * calling (invoking) other functions. 
 *
 * A Debugger-instance may be passed via the "SetDebugger(Debugger*)" method. 
 */
class FunctionInvocation : public ExecutionDelegate {
public:
	FunctionInvocation(Function *function, FunctionInvocation *caller);
	~FunctionInvocation();

	void Execute();

	/* Execution Delegate */
	void PerformFunctionCall(Function *function);
	void BranchToInstruction(unsigned index);
	void ReturnToCaller();

	/* Getter Methods  */
#ifdef _SCRAP_TEST_		
	// Allow modification in test environments
	Stack* GetMutableStack();	
#endif

	const Stack* GetStack() const;

	// Push return value (top stack value) onto the calling stack
	void ReturnValue();

	/* A Debugger-instance may be assigned to the FunctionInvocation. The Debugger
	 * will be passed down to any invocations invoked by this invocation.
	 */
	void SetDebugger(Debugger *debugger);


private:
	Function *_function;
	FunctionInvocation *_caller;

	Stack _stack;
	Executor _executor;

	Debugger *_debugger;

	unsigned _pc;
	bool _return;

	// Pop the required arguments from the caller stack and push them to own stack
	void TransferArguments();
};

}
