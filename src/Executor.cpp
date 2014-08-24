#include "Executor.h"
#include "Bytecode.h"
#include "Stack.h"


namespace scrap {

// Hash map of instructions to methods. The number of bytes consumed by 
// the instruction is returned. This map is global and built from the first
// Executor instance created.
static map<byte, ExecutorMethod> g_methodMap;


Executor::Executor(ExecutionDelegate *delegate, Stack *stack)
	:	_delegate(delegate),
		_stack(stack)
{
	if (g_methodMap.size() == 0) {
		BuildInstructionMap();
	}

	for (int i=0; i<NUM_REGISTERS; i++)
		_reg[i] = NULL;
}

Executor::~Executor()
{

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
	g_methodMap[OP_POP] 		= &Executor::Pop;
	g_methodMap[OP_COPY] 		= &Executor::Copy;
	g_methodMap[OP_RETURN] 		= &Executor::Return;

	g_methodMap[OP_I_LOAD]		= &Executor::ILoad;
    g_methodMap[OP_I_RETURN]	= &Executor::IReturn;
    g_methodMap[OP_I_STORE]		= &Executor::IStore;
    g_methodMap[OP_I_PUSH]		= &Executor::IPush;

	g_methodMap[OP_F_LOAD]		= &Executor::FLoad;
    g_methodMap[OP_F_RETURN]	= &Executor::FReturn;
    g_methodMap[OP_F_STORE]		= &Executor::FStore;
    g_methodMap[OP_F_PUSH]		= &Executor::FPush;

	g_methodMap[OP_D_LOAD]		= &Executor::DLoad;
    g_methodMap[OP_D_RETURN]	= &Executor::DReturn;
    g_methodMap[OP_D_STORE]		= &Executor::DStore;
    g_methodMap[OP_D_PUSH]		= &Executor::DPush;

	g_methodMap[OP_L_LOAD]		= &Executor::LLoad;
    g_methodMap[OP_L_RETURN]	= &Executor::LReturn;
    g_methodMap[OP_L_STORE]		= &Executor::LStore;
    g_methodMap[OP_L_PUSH]		= &Executor::LPush;

	g_methodMap[OP_C_LOAD]		= &Executor::CLoad;
    g_methodMap[OP_C_RETURN]	= &Executor::CReturn;
    g_methodMap[OP_C_STORE]		= &Executor::CStore;
    g_methodMap[OP_C_PUSH]		= &Executor::CPush;

	g_methodMap[OP_B_LOAD]		= &Executor::BLoad;
    g_methodMap[OP_B_RETURN]	= &Executor::BReturn;
    g_methodMap[OP_B_STORE]		= &Executor::BStore;
    g_methodMap[OP_B_PUSH]		= &Executor::BPush;

    g_methodMap[OP_F2D]			= &Executor::F2D;
    g_methodMap[OP_F2L]			= &Executor::F2L;
    g_methodMap[OP_F2I]			= &Executor::F2I;
    g_methodMap[OP_F2C]			= &Executor::F2C;
    g_methodMap[OP_F2B]			= &Executor::F2B;
    g_methodMap[OP_D2F]			= &Executor::D2F;
    g_methodMap[OP_D2L]			= &Executor::D2L;
    g_methodMap[OP_D2I]			= &Executor::D2I;
    g_methodMap[OP_D2C]			= &Executor::D2C;
    g_methodMap[OP_D2B]			= &Executor::D2B;
    g_methodMap[OP_L2I]			= &Executor::L2I;
    g_methodMap[OP_L2F]			= &Executor::L2F;
    g_methodMap[OP_L2D]			= &Executor::L2D;
    g_methodMap[OP_L2C]			= &Executor::L2C;
    g_methodMap[OP_L2B]			= &Executor::L2B;
    g_methodMap[OP_I2L]			= &Executor::I2L;
    g_methodMap[OP_I2F]			= &Executor::I2F;
    g_methodMap[OP_I2D]			= &Executor::I2D;
    g_methodMap[OP_I2C]			= &Executor::I2C;
    g_methodMap[OP_I2B]			= &Executor::I2B;
    g_methodMap[OP_C2I]			= &Executor::C2I;
    g_methodMap[OP_C2L]			= &Executor::C2L;
    g_methodMap[OP_C2F]			= &Executor::C2F;
    g_methodMap[OP_C2D]			= &Executor::C2D;
    g_methodMap[OP_C2B]			= &Executor::C2B;
    g_methodMap[OP_B2I]			= &Executor::B2I;
    g_methodMap[OP_B2F]			= &Executor::B2F;
    g_methodMap[OP_B2D]			= &Executor::B2D;
    g_methodMap[OP_B2L]			= &Executor::B2L;
    g_methodMap[OP_B2C]			= &Executor::B2C;

	g_methodMap[OP_I_ADD]		= &Executor::IAdd;
    g_methodMap[OP_I_SUB]		= &Executor::ISub;
    g_methodMap[OP_I_MUL]		= &Executor::IMul;
    g_methodMap[OP_I_DIV]		= &Executor::IDiv;
    g_methodMap[OP_F_ADD]		= &Executor::FAdd;
    g_methodMap[OP_F_SUB]		= &Executor::FSub;
    g_methodMap[OP_F_MUL]		= &Executor::FMul;
    g_methodMap[OP_F_DIV]		= &Executor::FDiv;
    g_methodMap[OP_D_ADD]		= &Executor::DAdd;
    g_methodMap[OP_D_SUB]		= &Executor::DSub;
    g_methodMap[OP_D_MUL]		= &Executor::DMul;
    g_methodMap[OP_D_DIV]		= &Executor::DDiv;
    g_methodMap[OP_L_ADD]		= &Executor::LAdd;
    g_methodMap[OP_L_SUB]		= &Executor::LSub;
    g_methodMap[OP_L_MUL]		= &Executor::LMul;
    g_methodMap[OP_L_DIV]		= &Executor::LDiv;
    g_methodMap[OP_C_ADD]		= &Executor::CAdd;
    g_methodMap[OP_C_SUB]		= &Executor::CSub;
    g_methodMap[OP_C_MUL]		= &Executor::CMul;
    g_methodMap[OP_C_DIV]		= &Executor::CDiv;
    g_methodMap[OP_I_SHL]		= &Executor::IShl;
    g_methodMap[OP_I_SHR]		= &Executor::IShr;
    g_methodMap[OP_I_MOD]		= &Executor::IMod;
    g_methodMap[OP_I_XOR]		= &Executor::IXor;
    g_methodMap[OP_I_AND]		= &Executor::IAnd;
    g_methodMap[OP_I_OR]		= &Executor::IOr;
    g_methodMap[OP_L_SHL]		= &Executor::LShl;
    g_methodMap[OP_L_SHR]		= &Executor::LShr;
    g_methodMap[OP_L_MOD]		= &Executor::LMod;
    g_methodMap[OP_L_XOR]		= &Executor::LXor;
    g_methodMap[OP_L_AND]		= &Executor::LAnd;
    g_methodMap[OP_L_OR]		= &Executor::LOr;
    g_methodMap[OP_C_SHL]		= &Executor::CShl;
    g_methodMap[OP_C_SHR]		= &Executor::CShr;
    g_methodMap[OP_C_MOD]		= &Executor::CMod;
    g_methodMap[OP_C_XOR]		= &Executor::CXor;
    g_methodMap[OP_C_AND]		= &Executor::CAnd;
    g_methodMap[OP_C_OR]		= &Executor::COr;

	g_methodMap[OP_CALL]		= &Executor::Call;

	g_methodMap[OP_BRANCH] 		= &Executor::Branch;
	g_methodMap[OP_BIFNULL] 	= &Executor::BifNull;
	g_methodMap[OP_BIFNOTNULL] 	= &Executor::BifNotNull;
	g_methodMap[OP_BIFGREATER] 	= &Executor::BifGreater;
	g_methodMap[OP_BIFGREATEREQ]= &Executor::BifGreaterEQ;
	g_methodMap[OP_BIFLESS] 	= &Executor::BifLess;
	g_methodMap[OP_BIFLESSEQ] 	= &Executor::BifLessEQ;
	g_methodMap[OP_BIFEQUAL] 	= &Executor::BifEqual;
	g_methodMap[OP_BIFNOTEQUAL] = &Executor::BifNotEqual;
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


unsigned Executor::Pop(const byte *instr) 
{
	Variable *var = _stack->Pop();
	delete var;

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

unsigned Executor::Return(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::ILoad(const byte *instr) 
{
	GenericLoad(VarType::i, instr[1]);
	return 2;
}

unsigned Executor::IReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::IStore(const byte *instr) 
{
	GenericStore(VarType::i, instr[1]);
	return 2;
}

unsigned Executor::IPush(const byte *instr) 
{
	int ival = *((int*)(instr + 1));
	Variable *var = new Variable(VarType::i);
	var->Set((int)ival);
	_stack->Push(var);
	return 5;
}

unsigned Executor::FLoad(const byte *instr) 
{
	GenericLoad(VarType::f, instr[1]);
	return 2;
}

unsigned Executor::FReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::FStore(const byte *instr) 
{
	GenericStore(VarType::f, instr[1]);
	return 2;
}

unsigned Executor::FPush(const byte *instr) 
{
	float fval = *((float*)(instr + 1));
	Variable *var = new Variable(VarType::f);
	var->Set((float)fval);
	_stack->Push(var);
	return 5;
}

unsigned Executor::DLoad(const byte *instr) 
{
	GenericLoad(VarType::d, instr[1]);
	return 2;
}

unsigned Executor::DReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::DStore(const byte *instr) 
{
	GenericStore(VarType::d, instr[1]);
	return 2;
}

unsigned Executor::DPush(const byte *instr) 
{
	double dval = *((double*)(instr + 1));
	Variable *var = new Variable(VarType::d);
	var->Set((double)dval);
	_stack->Push(var);
	return 9;
}

unsigned Executor::LLoad(const byte *instr) 
{
	GenericLoad(VarType::l, instr[1]);
	return 2;
}

unsigned Executor::LReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::LStore(const byte *instr) 
{
	GenericStore(VarType::l, instr[1]);
	return 2;
}

unsigned Executor::LPush(const byte *instr) 
{
	long lval = *((long*)(instr + 1));
	Variable *var = new Variable(VarType::l);
	var->Set((long)lval);
	_stack->Push(var);
	return 9;
}

unsigned Executor::CLoad(const byte *instr) 
{
	GenericLoad(VarType::c, instr[1]);
	return 2;
}

unsigned Executor::CReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::CStore(const byte *instr) 
{
	GenericStore(VarType::c, instr[1]);
	return 2;
}

unsigned Executor::CPush(const byte *instr) 
{
	char cval = *((char*)(instr + 1));
	Variable *var = new Variable(VarType::c);
	var->Set((char)cval);
	_stack->Push(var);
	return 2;
}

unsigned Executor::BLoad(const byte *instr) 
{
	GenericLoad(VarType::b, instr[1]);
	return 2;
}

unsigned Executor::BReturn(const byte *instr) 
{
	_delegate->ReturnToCaller();
	return 1;
}

unsigned Executor::BStore(const byte *instr) 
{
	GenericStore(VarType::b, instr[1]);
	return 2;
}

unsigned Executor::BPush(const byte *instr) 
{
	bool bval = *((bool*)(instr + 1));
	Variable *var = new Variable(VarType::b);
	var->Set((bool)bval);
	_stack->Push(var);
	return 2;
}

unsigned Executor::F2D(const byte *instr) 
{
	GenericConvert(VarType::f, VarType::d);
	return 1;
}

unsigned Executor::F2L(const byte *instr) 
{
	GenericConvert(VarType::f, VarType::l);
	return 1;
}

unsigned Executor::F2I(const byte *instr) 
{
	GenericConvert(VarType::f, VarType::i);
	return 1;
}

unsigned Executor::F2C(const byte *instr) 
{
	GenericConvert(VarType::f, VarType::c);
	return 1;
}

unsigned Executor::F2B(const byte *instr) 
{
	GenericConvert(VarType::f, VarType::b);
	return 1;
}

unsigned Executor::D2F(const byte *instr) 
{
	GenericConvert(VarType::d, VarType::f);
	return 1;
}

unsigned Executor::D2L(const byte *instr) 
{
	GenericConvert(VarType::d, VarType::l);
	return 1;
}

unsigned Executor::D2I(const byte *instr) 
{
	GenericConvert(VarType::d, VarType::i);
	return 1;
}

unsigned Executor::D2C(const byte *instr) 
{
	GenericConvert(VarType::d, VarType::c);
	return 1;
}

unsigned Executor::D2B(const byte *instr) 
{
	GenericConvert(VarType::d, VarType::b);
	return 1;
}

unsigned Executor::L2I(const byte *instr) 
{
	GenericConvert(VarType::l, VarType::i);
	return 1;
}

unsigned Executor::L2F(const byte *instr) 
{
	GenericConvert(VarType::l, VarType::f);
	return 1;
}

unsigned Executor::L2D(const byte *instr) 
{
	GenericConvert(VarType::l, VarType::d);
	return 1;
}

unsigned Executor::L2C(const byte *instr) 
{
	GenericConvert(VarType::l, VarType::c);
	return 1;
}

unsigned Executor::L2B(const byte *instr) 
{
	GenericConvert(VarType::l, VarType::b);
	return 1;
}

unsigned Executor::I2L(const byte *instr) 
{
	GenericConvert(VarType::i, VarType::l);
	return 1;
}

unsigned Executor::I2F(const byte *instr) 
{
	GenericConvert(VarType::i, VarType::f);
	return 1;
}

unsigned Executor::I2D(const byte *instr) 
{
	GenericConvert(VarType::i, VarType::d);
	return 1;
}

unsigned Executor::I2C(const byte *instr) 
{
	GenericConvert(VarType::i, VarType::c);
	return 1;
}

unsigned Executor::I2B(const byte *instr) 
{
	GenericConvert(VarType::i, VarType::b);
	return 1;
}

unsigned Executor::C2I(const byte *instr) 
{
	GenericConvert(VarType::c, VarType::i);
	return 1;
}

unsigned Executor::C2L(const byte *instr) 
{
	GenericConvert(VarType::c, VarType::l);
	return 1;
}

unsigned Executor::C2F(const byte *instr) 
{
	GenericConvert(VarType::c, VarType::f);
	return 1;
}

unsigned Executor::C2D(const byte *instr) 
{
	GenericConvert(VarType::c, VarType::d);
	return 1;
}

unsigned Executor::C2B(const byte *instr) 
{
	GenericConvert(VarType::c, VarType::b);
	return 1;
}

unsigned Executor::B2I(const byte *instr) 
{
	GenericConvert(VarType::b, VarType::i);
	return 1;
}

unsigned Executor::B2F(const byte *instr) 
{
	GenericConvert(VarType::b, VarType::f);
	return 1;
}

unsigned Executor::B2D(const byte *instr) 
{
	GenericConvert(VarType::b, VarType::d);
	return 1;
}

unsigned Executor::B2L(const byte *instr) 
{
	GenericConvert(VarType::b, VarType::l);
	return 1;
}

unsigned Executor::B2C(const byte *instr) 
{
	GenericConvert(VarType::b, VarType::c);
	return 1;
}

unsigned Executor::IAdd(const byte *instr) 
{
	GenericAdd(VarType::i);
	return 1;
}

unsigned Executor::ISub(const byte *instr) 
{
	GenericSub(VarType::i);
	return 1;
}

unsigned Executor::IMul(const byte *instr) 
{
	GenericMul(VarType::i);
	return 1;
}

unsigned Executor::IDiv(const byte *instr) 
{
	GenericDiv(VarType::i);
	return 1;
}

unsigned Executor::FAdd(const byte *instr) 
{
	GenericAdd(VarType::f);
	return 1;
}

unsigned Executor::FSub(const byte *instr) 
{
	GenericSub(VarType::f);
	return 1;
}

unsigned Executor::FMul(const byte *instr) 
{
	GenericMul(VarType::f);
	return 1;
}

unsigned Executor::FDiv(const byte *instr) 
{
	GenericDiv(VarType::f);
	return 1;
}

unsigned Executor::DAdd(const byte *instr) 
{
	GenericAdd(VarType::d);
	return 1;
}

unsigned Executor::DSub(const byte *instr) 
{
	GenericSub(VarType::d);
	return 1;
}

unsigned Executor::DMul(const byte *instr) 
{
	GenericMul(VarType::d);
	return 1;
}

unsigned Executor::DDiv(const byte *instr) 
{
	GenericDiv(VarType::d);
	return 1;
}

unsigned Executor::LAdd(const byte *instr) 
{
	GenericAdd(VarType::l);
	return 1;
}

unsigned Executor::LSub(const byte *instr) 
{
	GenericSub(VarType::l);
	return 1;
}

unsigned Executor::LMul(const byte *instr) 
{
	GenericMul(VarType::l);
	return 1;
}

unsigned Executor::LDiv(const byte *instr) 
{
	GenericDiv(VarType::l);
	return 1;
}

unsigned Executor::CAdd(const byte *instr) 
{
	GenericAdd(VarType::c);
	return 1;
}

unsigned Executor::CSub(const byte *instr) 
{
	GenericSub(VarType::c);
	return 1;
}

unsigned Executor::CMul(const byte *instr) 
{
	GenericMul(VarType::c);
	return 1;
}

unsigned Executor::CDiv(const byte *instr) 
{
	GenericDiv(VarType::c);
	return 1;
}

unsigned Executor::IShl(const byte *instr) 
{
	GenericShl(VarType::i);
	return 1;
}

unsigned Executor::IShr(const byte *instr) 
{
	GenericShr(VarType::i);
	return 1;
}

unsigned Executor::IMod(const byte *instr) 
{
	GenericMod(VarType::i);
	return 1;
}

unsigned Executor::IXor(const byte *instr) 
{
	GenericXor(VarType::i);
	return 1;
}

unsigned Executor::IAnd(const byte *instr) 
{
	GenericAnd(VarType::i);
	return 1;
}

unsigned Executor::IOr(const byte *instr) 
{
	GenericOr(VarType::i);
	return 1;
}

unsigned Executor::LShl(const byte *instr) 
{
	GenericShl(VarType::l);
	return 1;
}

unsigned Executor::LShr(const byte *instr) 
{
	GenericShr(VarType::l);
	return 1;
}

unsigned Executor::LMod(const byte *instr) 
{
	GenericMod(VarType::l);
	return 1;
}

unsigned Executor::LXor(const byte *instr) 
{
	GenericXor(VarType::l);
	return 1;
}

unsigned Executor::LAnd(const byte *instr) 
{
	GenericAnd(VarType::l);
	return 1;
}

unsigned Executor::LOr(const byte *instr) 
{
	GenericOr(VarType::l);
	return 1;
}

unsigned Executor::CShl(const byte *instr) 
{
	GenericShl(VarType::c);
	return 1;
}

unsigned Executor::CShr(const byte *instr) 
{
	GenericShr(VarType::c);
	return 1;
}

unsigned Executor::CMod(const byte *instr) 
{
	GenericMod(VarType::c);
	return 1;
}

unsigned Executor::CXor(const byte *instr) 
{
	GenericXor(VarType::c);
	return 1;
}

unsigned Executor::CAnd(const byte *instr) 
{
	GenericAnd(VarType::c);
	return 1;
}

unsigned Executor::COr(const byte *instr) 
{
	GenericOr(VarType::c);
	return 1;
}

unsigned Executor::Call(const byte *instr)
{
	unsigned *uptr = (unsigned*)(instr + 1);
	unsigned functionId = *uptr;
	
	_delegate->PerformFunctionCall(functionId);
	return 5;
}

unsigned Executor::Branch(const byte *instr) 
{	
	unsigned ip= *((unsigned*)(instr + 1));

	_delegate->BranchToInstruction(ip);

	return 0;
}

unsigned Executor::BifNull(const byte *instr) 
{
	Variable *cmp = new Variable();
	cmp->Set(0);
	_stack->Push(cmp);

	return ConditionalBranch(instr, &Variable::operator==);
}

unsigned Executor::BifNotNull(const byte *instr) 
{
	Variable *cmp = new Variable();
	cmp->Set(0);
	_stack->Push(cmp);

	return ConditionalBranch(instr, &Variable::operator!=);
}

unsigned Executor::BifGreater(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator>);
}

unsigned Executor::BifGreaterEQ(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator>=);
}

unsigned Executor::BifLess(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator<);
}

unsigned Executor::BifLessEQ(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator<=);
}

unsigned Executor::BifEqual(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator==);
}

unsigned Executor::BifNotEqual(const byte *instr) 
{
	return ConditionalBranch(instr, &Variable::operator!=);
}


void Executor::GenericAdd(VarType type)
{
	Variable *v2 = _stack->Pop();
	Variable *v1 = _stack->Pop();
	v1->Add(v2);

	_stack->Push(v1);
	
	delete v2;
}

void Executor::GenericSub(VarType type)
{
	Variable *v2 = _stack->Pop();
	Variable *v1 = _stack->Pop();
	v1->Sub(v2);
	
	_stack->Push(v1);
	
	delete v2;
}

void Executor::GenericMul(VarType type)
{
	Variable *v2 = _stack->Pop();
	Variable *v1 = _stack->Pop();
	v1->Mul(v2);
	
	_stack->Push(v1);
	
	delete v2;
}

void Executor::GenericDiv(VarType type)
{
	Variable *v2 = _stack->Pop();
	Variable *v1 = _stack->Pop();
	v1->Div(v2);
	
	_stack->Push(v1);
	
	delete v2;
}



void Executor::GenericMod(VarType type) 
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->Mod(var2);
	
	_stack->Push(var1);
	
	delete var2;
}

void Executor::GenericAnd(VarType type) 
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->And(var2);
	
	_stack->Push(var1);
	
	delete var2;
}

void Executor::GenericXor(VarType type)
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->Xor(var2);
	
	_stack->Push(var1);
	
	delete var2;
}

void Executor::GenericOr(VarType type) 
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->Or(var2);
	
	_stack->Push(var1);
	
	delete var2;
}

void Executor::GenericShl(VarType type) 
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->Shl(var2);
	
	_stack->Push(var1);
	
	delete var2;
}

void Executor::GenericShr(VarType type) 
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();
	var1->Shr(var2);
	
	_stack->Push(var1);
	
	delete var2;
}


unsigned Executor::ConditionalBranch(const byte *instr, VariableCompMethod method)
{
	Variable *var2 = _stack->Pop();
	Variable *var1 = _stack->Pop();

	bool condition = (*var1.*method)(var2);

	delete var2;
	delete var1;

	if (condition) {
		Branch(instr);
		return 0;
	}

	return 5;
}


void Executor::GenericLoad(VarType type, byte regIdx)
{
	Variable *&reg = GetRegister(regIdx);
	_stack->Push(reg);
}

void Executor::GenericStore(VarType type, byte regIdx)
{
	Variable *&reg = GetRegister(regIdx);
	Variable *var = _stack->Pop();
	reg = var;
}


void Executor::GenericConvert( VarType from, VarType to)
{
	Variable *var = _stack->Pop();

	if (var->Type() != from) {
		THROW(InvalidTypeException, "Conversion expected top value of "
			+ VarTypeToString(from) + " but received variable of type "
			+ VarTypeToString(var->Type()) + " (attempted to convert to "
			+ VarTypeToString(to));
	}

	var->Cast(to);
	_stack->Push(var);
}

}
