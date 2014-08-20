#pragma once

#include "Scrap.h"
#include "Stack.h"
#include "Executor.h"


namespace scrap {

class Function;
class Stack;
class Debugger;
class Program;


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
	/* Default constructor. 
	 * @param program
	 * 		In cases where the function is not fully self contained (no	access 
	 * 		to global variables, no function calls), it can be NULL.
	 * @param function
	 * 		The function to be invoked.
	 * @param caller
	 * 		The calling function, if applicable. If not NULL, the expected arguments
	 * 		are pushed onto the constructing invocations' stack.
	 */
	FunctionInvocation(Program *program, Function *function, FunctionInvocation *caller);

	~FunctionInvocation();

	void Execute();

	/* Execution Delegate */
	void PerformFunctionCall(unsigned funcIndex);
	void BranchToInstruction(unsigned index);
	void ReturnToCaller();

	/* Getter Methods  */
#ifdef _SCRAP_TEST_		
	// Allow modification in test environments
	Stack* GetMutableStack();	
#endif

	const Stack* GetStack() const;
	const Function* GetFunction() const;

	// Push return value (top stack value) onto the calling stack
	void ReturnValue();

	/* A Debugger-instance may be assigned to the FunctionInvocation. The Debugger
	 * will be passed down to any invocations invoked by this invocation.
	 */
	void SetDebugger(Debugger *debugger);


private:
	Program *_program;
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
