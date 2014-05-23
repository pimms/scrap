#include "Executor.h"
#include "Bytecode.h"
#include "Stack.h"
#include "Heap.h"
#include "Variable.h"
#include "Object.h"
#include "Class.h"


namespace scrap {

// Hash map of instructions to methods. The number of bytes consumed by 
// the instruction is returned. This map is global and built from the first
// Executor instance created.
static map<byte, ExecutorMethod> g_methodMap;


Executor::Executor(ExecutionDelegate *delegate, Stack *stack, Heap *heap)
	:	_delegate(delegate),
		_stack(stack),
		_heap(heap)
{
	if (g_methodMap.size() == 0) {
		BuildInstructionMap();
	}
}


unsigned Executor::Execute(const byte *instr)
{
	byte op = instr[0];
	unsigned len = arglen(op);

	ExecutorMethod method = GetMethod(op);
	if (!method) {
		THROW(InvalidOperationException, "Instruction not implemented");
	}

	return (*this.*method)(instr);
}



void Executor::BuildInstructionMap()
{
	g_methodMap[OP_POP] 		= NULL;
	g_methodMap[OP_COPY] 		= NULL;
	g_methodMap[OP_ARRAYLENGTH]	= NULL;
	g_methodMap[OP_ARRAYLOAD] 	= NULL;
	g_methodMap[OP_RETURN] 		= NULL;

	g_methodMap[OP_A_LOAD]		= NULL;
	g_methodMap[OP_A_RETURN]	= NULL;
	g_methodMap[OP_A_STORE]		= NULL;
	g_methodMap[OP_A_NEWARRAY]	= NULL;
	g_methodMap[OP_A_ARELEASE]	= NULL;
	g_methodMap[OP_A_ALOAD]		= NULL;
	g_methodMap[OP_A_ASTORE]	= NULL;

	g_methodMap[OP_I_LOAD]		= NULL;
    g_methodMap[OP_I_RETURN]	= NULL;
    g_methodMap[OP_I_STORE]		= NULL;
    g_methodMap[OP_I_PUSH]		= NULL;
    g_methodMap[OP_I_NEWARRAY]	= NULL;
    g_methodMap[OP_I_ARELEASE]	= NULL;
    g_methodMap[OP_I_ALOAD]		= NULL;
    g_methodMap[OP_I_ASTORE]	= NULL;

	g_methodMap[OP_F_LOAD]		= NULL;
    g_methodMap[OP_F_RETURN]	= NULL;
    g_methodMap[OP_F_STORE]		= NULL;
    g_methodMap[OP_F_PUSH]		= NULL;
    g_methodMap[OP_F_NEWARRAY]	= NULL;
    g_methodMap[OP_F_ARELEASE]	= NULL;
    g_methodMap[OP_F_ALOAD]		= NULL;
    g_methodMap[OP_F_ASTORE]	= NULL;

	g_methodMap[OP_D_LOAD]		= NULL;
    g_methodMap[OP_D_RETURN]	= NULL;
    g_methodMap[OP_D_STORE]		= NULL;
    g_methodMap[OP_D_PUSH]		= NULL;
    g_methodMap[OP_D_NEWARRAY]	= NULL;
    g_methodMap[OP_D_ARELEASE]	= NULL;
    g_methodMap[OP_D_ALOAD]		= NULL;
    g_methodMap[OP_D_ASTORE]	= NULL;

	g_methodMap[OP_L_LOAD]		= NULL;
    g_methodMap[OP_L_RETURN]	= NULL;
    g_methodMap[OP_L_STORE]		= NULL;
    g_methodMap[OP_L_PUSH]		= NULL;
    g_methodMap[OP_L_NEWARRAY]	= NULL;
    g_methodMap[OP_L_ARELEASE]	= NULL;
    g_methodMap[OP_L_ALOAD]		= NULL;
    g_methodMap[OP_L_ASTORE]	= NULL;

	g_methodMap[OP_C_LOAD]		= NULL;
    g_methodMap[OP_C_RETURN]	= NULL;
    g_methodMap[OP_C_STORE]		= NULL;
    g_methodMap[OP_C_PUSH]		= NULL;
    g_methodMap[OP_C_NEWARRAY]	= NULL;
    g_methodMap[OP_C_ARELEASE]	= NULL;
    g_methodMap[OP_C_ALOAD]		= NULL;
	g_methodMap[OP_C_ASTORE] 	= NULL;

	g_methodMap[OP_B_LOAD]		= NULL;
    g_methodMap[OP_B_RETURN]	= NULL;
    g_methodMap[OP_B_STORE]		= NULL;
    g_methodMap[OP_B_PUSH]		= NULL;
    g_methodMap[OP_B_NEWARRAY]	= NULL;
    g_methodMap[OP_B_ARELEASE]	= NULL;
    g_methodMap[OP_B_ALOAD]		= NULL;
    g_methodMap[OP_B_ASTORE]	= NULL;
    g_methodMap[OP_B_ASTORE]	= NULL;

	g_methodMap[OP_A2B]			= NULL;
    g_methodMap[OP_F2D]			= NULL;
    g_methodMap[OP_F2L]			= NULL;
    g_methodMap[OP_F2I]			= NULL;
    g_methodMap[OP_F2C]			= NULL;
    g_methodMap[OP_F2B]			= NULL;
    g_methodMap[OP_D2F]			= NULL;
    g_methodMap[OP_D2L]			= NULL;
    g_methodMap[OP_D2I]			= NULL;
    g_methodMap[OP_D2C]			= NULL;
    g_methodMap[OP_D2B]			= NULL;
    g_methodMap[OP_L2I]			= NULL;
    g_methodMap[OP_L2F]			= NULL;
    g_methodMap[OP_L2D]			= NULL;
    g_methodMap[OP_L2C]			= NULL;
    g_methodMap[OP_L2B]			= NULL;
    g_methodMap[OP_I2L]			= NULL;
    g_methodMap[OP_I2F]			= NULL;
    g_methodMap[OP_I2D]			= NULL;
    g_methodMap[OP_I2C]			= NULL;
    g_methodMap[OP_I2B]			= NULL;
    g_methodMap[OP_C2I]			= NULL;
    g_methodMap[OP_C2L]			= NULL;
    g_methodMap[OP_C2F]			= NULL;
    g_methodMap[OP_C2D]			= NULL;
    g_methodMap[OP_C2B]			= NULL;
    g_methodMap[OP_B2I]			= NULL;
    g_methodMap[OP_B2F]			= NULL;
    g_methodMap[OP_B2D]			= NULL;
    g_methodMap[OP_B2L]			= NULL;
    g_methodMap[OP_B2C]			= NULL;

	g_methodMap[OP_I_ADD]		= NULL;
    g_methodMap[OP_I_SUB]		= NULL;
    g_methodMap[OP_I_MUL]		= NULL;
    g_methodMap[OP_I_DIV]		= NULL;
    g_methodMap[OP_F_ADD]		= NULL;
    g_methodMap[OP_F_SUB]		= NULL;
    g_methodMap[OP_F_MUL]		= NULL;
    g_methodMap[OP_F_DIV]		= NULL;
    g_methodMap[OP_D_ADD]		= NULL;
    g_methodMap[OP_D_SUB]		= NULL;
    g_methodMap[OP_D_MUL]		= NULL;
    g_methodMap[OP_D_DIV]		= NULL;
    g_methodMap[OP_L_ADD]		= NULL;
    g_methodMap[OP_L_SUB]		= NULL;
    g_methodMap[OP_L_MUL]		= NULL;
    g_methodMap[OP_L_DIV]		= NULL;
    g_methodMap[OP_C_ADD]		= NULL;
    g_methodMap[OP_C_SUB]		= NULL;
    g_methodMap[OP_C_MUL]		= NULL;
    g_methodMap[OP_C_DIV]		= NULL;
    g_methodMap[OP_I_SHL]		= NULL;
    g_methodMap[OP_I_SHR]		= NULL;
    g_methodMap[OP_I_MOD]		= NULL;
    g_methodMap[OP_I_XOR]		= NULL;
    g_methodMap[OP_I_AND]		= NULL;
    g_methodMap[OP_I_OR]		= NULL;
    g_methodMap[OP_L_SHL]		= NULL;
    g_methodMap[OP_L_SHR]		= NULL;
    g_methodMap[OP_L_MOD]		= NULL;
    g_methodMap[OP_L_XOR]		= NULL;
    g_methodMap[OP_L_AND]		= NULL;
    g_methodMap[OP_L_OR]		= NULL;
    g_methodMap[OP_C_SHL]		= NULL;
    g_methodMap[OP_C_SHR]		= NULL;
    g_methodMap[OP_C_MOD]		= NULL;
    g_methodMap[OP_C_XOR]		= NULL;
    g_methodMap[OP_C_AND]		= NULL;
    g_methodMap[OP_C_OR]		= NULL;

	g_methodMap[OP_NEW] 		= NULL;
	g_methodMap[OP_RETAIN] 		= NULL;
	g_methodMap[OP_RELEASE] 	= NULL;
	g_methodMap[OP_INVOKE] 		= NULL;
	g_methodMap[OP_VINVOKE] 	= NULL;
	g_methodMap[OP_STINVOKE] 	= NULL;
	g_methodMap[OP_LOADFIELD] 	= NULL;
	g_methodMap[OP_LOADSTATIC] 	= NULL;
	g_methodMap[OP_STOREFIELD] 	= NULL;
	g_methodMap[OP_STORESTATIC]	= NULL;

	g_methodMap[OP_BRANCH] 		= NULL;
	g_methodMap[OP_BIFNULL] 	= NULL;
	g_methodMap[OP_BIFNOTNULL] 	= NULL;
	g_methodMap[OP_BIFGREATER] 	= NULL;
	g_methodMap[OP_BIFGREATEREQ]= NULL;
	g_methodMap[OP_BIFLESS] 	= NULL;
	g_methodMap[OP_BIFLESSEQ] 	= NULL;
	g_methodMap[OP_BIFEQUAL] 	= NULL;
	g_methodMap[OP_BIFNOTEQUAL] = NULL;
}

ExecutorMethod Executor::GetMethod(byte instr)
{
	if (!g_methodMap.count(instr)) {
		return NULL;
	}

	return g_methodMap[instr];
}


Variable*& Executor::GetRegister(byte reg)
{
	if (reg >= 0 && reg < NUM_REGISTERS) {
		return _reg[reg];
	}
	
	THROW(IndexOutOfRangeException, "Attempted to access invalid register");
}




// Asserts that a variable (_VAR) is of type _T (short hand notation, i,a,
// d,f etc). 
#define ASSERT_TYPE(_VAR, _T) 							\
	if (_VAR->Type() != VarType::_T) 					\
		THROW(InvalidTypeException, "Type assertion "	\
		"failed. Expected " + VarTypeToString(_T) +		\
		", got " + VarTypeToString(_VAR->Type()));

unsigned Executor::Pop(const byte *instr) 
{
	Variable *var = _stack->Pop();
	if (!var->IsFieldVariable()) {
		delete var;
	}

	return 1;
}

unsigned Executor::Copy(const byte *instr) 
{
	Variable *var = _stack->Pop();
	Variable *cpy = var->Copy();

	_stack->Push(var);
	_stack->Push(cpy);

	return 1;
}

unsigned Executor::ArrayLength(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::ArrayLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::Return(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::ALoad(const byte *instr) 
{
	Variable *reg = GetRegister(instr[1]);
	ASSERT_TYPE(reg, a);
	_stack->Push(reg);

	return 2;
}

unsigned Executor::AReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::AStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::ANewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::AArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::AAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::AAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::ILoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::INewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FNewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DNewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LNewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CNewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BLoad(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BReturn(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BStore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BPush(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BNewarray(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BArelease(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BAload(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BAstore(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::A2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::F2D(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::F2L(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::F2I(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::F2C(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::F2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::D2F(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::D2L(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::D2I(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::D2C(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::D2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::L2I(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::L2F(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::L2D(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::L2C(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::L2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::I2L(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::I2F(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::I2D(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::I2C(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::I2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::C2I(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::C2L(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::C2F(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::C2D(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::C2B(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::B2I(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::B2F(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::B2D(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::B2L(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::B2C(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IAdd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::ISub(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IMul(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IDiv(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FAdd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FSub(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FMul(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::FDiv(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DAdd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DSub(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DMul(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::DDiv(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LAdd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LSub(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LMul(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LDiv(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CAdd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CSub(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CMul(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CDiv(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IShl(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IShr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IMod(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IXor(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IAnd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::IOr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LShl(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LShr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LMod(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LXor(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LAnd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LOr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CShl(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CShr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CMod(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CXor(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::CAnd(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::COr(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::New(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::Retain(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::Release(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::Invoke(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::VInvoke(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::STInvoke(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LoadField(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::LoadStatic(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::StoreField(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::StoreStatic(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::Branch(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifNull(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifNotNull(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifGreater(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifGreaterEQ(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifLess(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}

unsigned Executor::BifLessEQ(const byte *instr) 
{
	THROW(NotImplementedException, "Instruction method not implemented");
}


}
