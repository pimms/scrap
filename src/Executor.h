#pragma once

#include "Scrap.h"


namespace scrap {

class Variable;

class Variable;
class Stack;
class Executor;
class Function;

const int NUM_REGISTERS = 4;

typedef unsigned(Executor::*ExecutorMethod)(const byte*);


/* ExecutionDelegate
 * Callback channel for performing non-trivial instructions. 
 * For now only implemented by FunctionInvocation.
 */
class ExecutionDelegate {
public:
	virtual void PerformFunctionCall(Function *function) = 0;
	virtual void BranchToInstruction(unsigned index) = 0;
	virtual void ReturnToCaller() = 0;
};


/* Executor
 * The lowest level execution unit. Responsible for "blind" execution of
 * single instructions, performing modifications on the Stack and the Heap.
 */
class Executor {
public:
	Executor(ExecutionDelegate *delegate, Stack *stack);
	~Executor();
	
	/* The instr-pointer points to an arbitrary location inside the method's
	 * body. The method returns the number of bytes consumed by the instruction.
	 */
	unsigned Execute(const byte *instr);

private:
	ExecutionDelegate *_delegate;
	Stack *_stack;

	Variable *_reg[NUM_REGISTERS];

	void BuildInstructionMap();
	ExecutorMethod GetMethod(byte instr);

	Variable*& GetRegister(byte reg);

	unsigned Pop(const byte *instr);
	unsigned Copy(const byte *instr);
	unsigned Return(const byte *instr);
	unsigned ILoad(const byte *instr);
	unsigned IReturn(const byte *instr);
	unsigned IStore(const byte *instr);
	unsigned IPush(const byte *instr);
	unsigned FLoad(const byte *instr);
	unsigned FReturn(const byte *instr);
	unsigned FStore(const byte *instr);
	unsigned FPush(const byte *instr);
	unsigned DLoad(const byte *instr);
	unsigned DReturn(const byte *instr);
	unsigned DStore(const byte *instr);
	unsigned DPush(const byte *instr);
	unsigned LLoad(const byte *instr);
	unsigned LReturn(const byte *instr);
	unsigned LStore(const byte *instr);
	unsigned LPush(const byte *instr);
	unsigned CLoad(const byte *instr);
	unsigned CReturn(const byte *instr);
	unsigned CStore(const byte *instr);
	unsigned CPush(const byte *instr);
	unsigned BLoad(const byte *instr);
	unsigned BReturn(const byte *instr);
	unsigned BStore(const byte *instr);
	unsigned BPush(const byte *instr);
	unsigned F2D(const byte *instr);
	unsigned F2L(const byte *instr);
	unsigned F2I(const byte *instr);
	unsigned F2C(const byte *instr);
	unsigned F2B(const byte *instr);
	unsigned D2F(const byte *instr);
	unsigned D2L(const byte *instr);
	unsigned D2I(const byte *instr);
	unsigned D2C(const byte *instr);
	unsigned D2B(const byte *instr);
	unsigned L2I(const byte *instr);
	unsigned L2F(const byte *instr);
	unsigned L2D(const byte *instr);
	unsigned L2C(const byte *instr);
	unsigned L2B(const byte *instr);
	unsigned I2L(const byte *instr);
	unsigned I2F(const byte *instr);
	unsigned I2D(const byte *instr);
	unsigned I2C(const byte *instr);
	unsigned I2B(const byte *instr);
	unsigned C2I(const byte *instr);
	unsigned C2L(const byte *instr);
	unsigned C2F(const byte *instr);
	unsigned C2D(const byte *instr);
	unsigned C2B(const byte *instr);
	unsigned B2I(const byte *instr);
	unsigned B2F(const byte *instr);
	unsigned B2D(const byte *instr);
	unsigned B2L(const byte *instr);
	unsigned B2C(const byte *instr);
	unsigned IAdd(const byte *instr);
	unsigned ISub(const byte *instr);
	unsigned IMul(const byte *instr);
	unsigned IDiv(const byte *instr);
	unsigned FAdd(const byte *instr);
	unsigned FSub(const byte *instr);
	unsigned FMul(const byte *instr);
	unsigned FDiv(const byte *instr);
	unsigned DAdd(const byte *instr);
	unsigned DSub(const byte *instr);
	unsigned DMul(const byte *instr);
	unsigned DDiv(const byte *instr);
	unsigned LAdd(const byte *instr);
	unsigned LSub(const byte *instr);
	unsigned LMul(const byte *instr);
	unsigned LDiv(const byte *instr);
	unsigned CAdd(const byte *instr);
	unsigned CSub(const byte *instr);
	unsigned CMul(const byte *instr);
	unsigned CDiv(const byte *instr);
	unsigned IShl(const byte *instr);
	unsigned IShr(const byte *instr);
	unsigned IMod(const byte *instr);
	unsigned IXor(const byte *instr);
	unsigned IAnd(const byte *instr);
	unsigned IOr(const byte *instr);
	unsigned LShl(const byte *instr);
	unsigned LShr(const byte *instr);
	unsigned LMod(const byte *instr);
	unsigned LXor(const byte *instr);
	unsigned LAnd(const byte *instr);
	unsigned LOr(const byte *instr);
	unsigned CShl(const byte *instr);
	unsigned CShr(const byte *instr);
	unsigned CMod(const byte *instr);
	unsigned CXor(const byte *instr);
	unsigned CAnd(const byte *instr);
	unsigned COr(const byte *instr);
	unsigned Branch(const byte *instr);
	unsigned BifNull(const byte *instr);
	unsigned BifNotNull(const byte *instr);
	unsigned BifGreater(const byte *instr);
	unsigned BifGreaterEQ(const byte *instr);
	unsigned BifLess(const byte *instr);
	unsigned BifLessEQ(const byte *instr);
	unsigned BifEqual(const byte *instr);
	unsigned BifNotEqual(const byte *instr);
	
	// Generic operations, performs typed instructions
	// and asserts that the types match.
	void GenericAdd(VarType type);
	void GenericSub(VarType type);
	void GenericMul(VarType type);
	void GenericDiv(VarType type);

	void GenericMod(VarType type);
	void GenericAnd(VarType type);
	void GenericXor(VarType type);
	void GenericOr(VarType type);
	void GenericShl(VarType type);
	void GenericShr(VarType type);
	
	void GenericLoad(VarType type, byte regIdx);
	void GenericStore(VarType type, byte regIdx);

	void GenericConvert(VarType from, VarType to);
};
}
