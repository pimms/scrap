#pragma once

#include "Scrap.h"

namespace scrap {

class MethodInvocation;
class Variable;


/* Debugger
 * Abstract interface that will allow detailed information to be analyzed during
 * execution. Only byte-level debugging is possible, and the Scrap-code that generated
 * the executing bytecode is not regarded in any way. (This is mainly because the 
 * Scrap syntax is currently undefined).
 *
 * To attach a Debugger to a program, call "SetDebugger(Debugger*)" on a MethodInvocation
 * instance. All subsequent method-invocations invoked by this instance will use the same
 * Debugger.
 *
 * The program execution is blocked until the Debugger returns from it's callback methods.
 */
class Debugger {
public:
	virtual void WillExecuteInstruction(const MethodInvocation *invoc, const byte *instr) = 0;
	virtual void DidInvokeNewMethod(const MethodInvocation *invoc, const MethodInvocation *oldInvoc) { /* ... */ };
	virtual void DidReturn(const MethodInvocation *invoc, const Variable *returnVariable) { /* ... */ };
	
protected:
	virtual string GetMethodSignature(const MethodInvocation *invoc);
};



/* CLISimpleDebugger
 * The CLISimpleDebugger stops at every instruction to review the stack, heap and current instruction.
 * 
 */
class CLISimpleDebugger : public Debugger {
public:
	CLISimpleDebugger();
	~CLISimpleDebugger();

	void WillExecuteInstruction(const MethodInvocation *invoc, const byte *instr);
	void DidInvokeNewMethod(const MethodInvocation *invoc, const MethodInvocation *oldInvoc);
	void DidReturn(const MethodInvocation *invoc, const Variable *returnVariable);

private:
	void TakeCommands(const MethodInvocation *invoc);

	void PrintStackContents(const MethodInvocation *invoc);
};

}

