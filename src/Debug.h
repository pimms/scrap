#pragma once

#include "Scrap.h"

namespace scrap {

class FunctionInvocation;
class Variable;


/* Debugger
 * Abstract interface that will allow detailed information to be analyzed during
 * execution. Only byte-level debugging is possible, and the Scrap-code that generated
 * the executing bytecode is not regarded in any way. (This is mainly because the 
 * Scrap syntax is currently undefined).
 *
 * To attach a Debugger to a program, call "SetDebugger(Debugger*)" on a FunctionInvocation
 * instance. All subsequent method-invocations invoked by this instance will use the same
 * Debugger.
 *
 * The program execution is blocked until the Debugger returns from it's callback methods.
 */
class Debugger {
public:
	virtual void WillExecuteInstruction(const FunctionInvocation *invoc, const byte *instr) = 0;
	virtual void DidInvokeNewFunction(const FunctionInvocation *invoc, 
									const FunctionInvocation *oldInvoc) { /* ... */ };
	virtual void DidReturn(const FunctionInvocation *invoc, const Variable *returnVariable) { /* ... */ };
	
protected:
	virtual string GetFunctionSignature(const FunctionInvocation *invoc);
};



/* CLISimpleDebugger
 * The CLISimpleDebugger stops at every instruction to review the stack, heap and current instruction.
 * 
 */
class CLISimpleDebugger : public Debugger {
public:
	CLISimpleDebugger();
	~CLISimpleDebugger();

	void WillExecuteInstruction(const FunctionInvocation *invoc, const byte *instr);
	void DidInvokeNewFunction(const FunctionInvocation *invoc, const FunctionInvocation *oldInvoc);
	void DidReturn(const FunctionInvocation *invoc, const Variable *returnVariable);

private:
	void TakeCommands(const FunctionInvocation *invoc);

	void PrintStackContents(const FunctionInvocation *invoc);
};

}

