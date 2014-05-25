#include "ScrapTest.h"

/* Testing Implementation Overview
 *
 * [X] Pop
 * [X] Copy
 * [ ] ArrayLength
 * [ ] ArrayLoad
 * [X] Return
 * [ ] T Load
 * [ ] T Return
 * [ ] T Store
 * [X] T Push
 * [ ] T NewArray
 * [ ] T ARelease
 * [ ] T ALoad
 * [ ] T AStore
 * [ ] A2B
 * [ ] F2T
 * [ ] D2T
 * [ ] L2T
 * [ ] I2T
 * [ ] C2T
 * [ ] B2T
 * [X] T Add
 * [X] T Sub
 * [X] T Mul
 * [X] T Div
 * [X] T Shl
 * [X] T Shr
 * [X] T Mod
 * [X] T Xor
 * [X] T And
 * [X] T Or
 * [ ] New
 * [ ] Retain
 * [ ] Release
 * [ ] Invoke
 * [ ] VInvoke
 * [ ] STInvoke
 * [ ] LoadField
 * [ ] LoadStatic
 * [ ] StoreField
 * [ ] StoreStatic
 * [ ] Branch
 * [ ] BifNull
 * [ ] BifNotNull
 * [ ] BifGreater
 * [ ] BifGreaterEq
 * [ ] BifLess
 * [ ] BifLessEq
 * [ ] BifEqual
 * [ ] BifNotEqual
 */


/* The test program declares a program with a single class, MainClass and 
 * a single method, "void Main()". The bytecode-length and bytecode is not
 * included in the file and must be added before attempting to execute
 * the program.
 */
#define PROGRAM_BASE_FILE "testfiles/testprog_base"

/* To read the program file successfully, this class overrides 
 * ProgramParser but returns a predefined prgram file rather than
 * actually reading the file. This class will only work properly
 * when reading the test-program base file. 
 */
class TestProgramCreator : public ProgramParser {
public:
	TestProgramCreator(MethodBody body) {
		_body = body;
	}	

	MethodBody ReadMethodBody() {
		return _body;
	}

private:
	MethodBody _body;
};

Program* CreateProgram(MethodBody body)
{
	TestProgramCreator creator(body);
	return creator.ParseProgramFile(PROGRAM_BASE_FILE);
}

/* To test the program, the Program class can either be used directly
 * or the methods can be invoked manually. The latter gives us a view
 * into the Stack and Heap objects as they are not managed by the 
 * Program instance. 
 *
 * The method is extracted from the Program and executed manually.
 *
 * Note that the contents of the stack, the Stack-instance itself, 
 * and the heap contents must be deleted by the caller.
 */
Stack* ExecuteProgram(Program *program, Heap *heap)
{
	ClassList *clist = program->GetClassList();
	Class *mainClass = clist->GetClass(0);
	Method *mainMethod = mainClass->GetStaticMethod(0);

	MethodInvocation invocation(heap, mainMethod, mainClass, NULL);
	invocation.Execute();

	// Copy the stack
	Stack *stack = invocation.GetStack();
	Stack *copy = stack->Copy();
	
	// Pop the stack to prevent StackNotEmptyException
	while (stack->Count())
		stack->Pop();

	return copy;
}


/* Test pushing a literal value of type T. As there is no link between
 * the type and the instruction, the instruction must be passed as well.
 * To assert that the value is identical, the type getter method must be
 * passed as well.
 */
template<typename T>
void TestPushLiteral(byte instr, T val, T(Variable::*valmethod)()const)
{
	/* Push a single literal T. The stack should contain a single variable
	 * holding the value "T val".
	 *
	 * 		Tpush <val>
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	Variable *var = NULL;
	MethodBody body;

	body.length = 1 + sizeof(T);
	body.code = new byte[body.length];
	body.code[0] = instr;
	memcpy(body.code+1, &val, sizeof(T));

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 1);
	
	var = stack->Pop();	
	ASSERT_EQ(val, (*var.*valmethod)());
	
	delete var;
	delete stack;
	delete program;
}

template<typename T>
void TestArithmetic(byte pushInstr, byte aritInstr, T val1, T val2, 
					 void(Variable::*opmethod)(const Variable*), 
					 T(Variable::*valmethod)()const)
{
	/* Push two variables and perform arithmetic on them.
	 *
	 * 		Tpush <val1>
	 * 		Tpush <val2>
	 * 		<aritInstr>
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	Variable *var = NULL;
	MethodBody body;

	body.length = 3 + 2*sizeof(T);
	body.code = new byte[body.length];
	body.code[0] = pushInstr;
	memcpy(body.code+1, &val1, sizeof(T));
	body.code[1+sizeof(T)] = pushInstr;
	memcpy(body.code+2+sizeof(T), &val2, sizeof(T));
	body.code[2+2*sizeof(T)] = aritInstr;
	
	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 1);
	Variable *result = stack->Pop();

	Variable *v1 = new Variable;
	Variable *v2 = new Variable;
	v1->Set((T)val1);
	v2->Set((T)val2);
	(*v1.*opmethod)(v2);

	ASSERT_EQ(v1->Type(), result->Type());
	ASSERT_EQ((*v1.*valmethod)(), (*result.*valmethod)());

	delete result;
	delete v1;
	delete v2;
	delete stack;
	delete program;
}

TEST (ExecutorTest, TestPushLiteral)
{
	TestPushLiteral<int>(OP_I_PUSH, 0x8badf00d, &Variable::Value_i);
	TestPushLiteral<float>(OP_F_PUSH, 0x8badf00d, &Variable::Value_f);
	TestPushLiteral<long>(OP_L_PUSH, 0x8badf00ddeadbeef, &Variable::Value_l);
	TestPushLiteral<double>(OP_D_PUSH, 0x8badf00ddeadbeef, &Variable::Value_d);
	TestPushLiteral<char>(OP_C_PUSH, 0xFE, &Variable::Value_c);
	TestPushLiteral<bool>(OP_B_PUSH, 0x01, &Variable::Value_b);
}

TEST (ExecutorTest, TestPop)
{
	/* Push a value and pop it. The stack should be empty after execution.
	 *
	 * 		ipush 1337
	 * 		pop
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	int ival = 1337;

	body.length = 2 + sizeof(int);
	body.code = new byte[body.length];
	body.code[0] = OP_I_PUSH;
	memcpy(body.code+1, &ival, sizeof(int));
	body.code[1+sizeof(int)] = OP_POP;

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 0);
	
	delete stack;
	delete program;
}

TEST (ExecutorTest, TestCopy)
{
	/* Push an int-value and copy it. The stack should after execution
	 * contain two Variable objects with the same type and value.
	 *
	 * 		ipush 1337
	 * 		copy
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	int ival = 1337;

	body.length = 2 + sizeof(int);
	body.code = new byte[body.length];
	body.code[0] = OP_I_PUSH;
	memcpy(body.code+1, &ival, sizeof(int));
	body.code[1+sizeof(int)] = OP_COPY;

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 2);

	Variable *v1, *v2;
	v1 = stack->Pop();
	v2 = stack->Pop();

	ASSERT_EQ(v1->Type(), VarType::INT);
	ASSERT_EQ(v2->Type(), VarType::INT);
	ASSERT_EQ(v1->Value_i(), v2->Value_i());

	delete v1;
	delete v2;
	delete stack;
	delete program;
}

TEST (ExecutorTest, TestVoidReturn)
{
	/* Return and THEN push a float value. The float should therefore never 
	 * be pushed, and the stack should be empty post-execution.
	 *
	 * 		return
	 * 		fpush 	3.14
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	float fval = 3.14;

	body.length = 2 + sizeof(float);
	body.code = new byte[body.length];
	body.code[0] = OP_RETURN;
	body.code[1] = OP_F_PUSH;
	memcpy(body.code+2, &fval, sizeof(float));

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 0);

	delete stack;
	delete program;
}

TEST (ExecutorTest, TestAdd)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_ADD, 154645, 39483, 
					&Variable::Add, &Variable::Value_i);
	TestArithmetic<float>(OP_F_PUSH, OP_F_ADD, 3418.491f, 9384.184f, 
					&Variable::Add, &Variable::Value_f);
	TestArithmetic<double>(OP_D_PUSH, OP_D_ADD, 3418.491, 9384.184, 
					&Variable::Add, &Variable::Value_d);
	TestArithmetic<long>(OP_L_PUSH, OP_L_ADD, 341482738, 93482184, 
					&Variable::Add, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_ADD, 100, 112, 
					&Variable::Add, &Variable::Value_c);
}

TEST (ExecutorTest, TestSub)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_SUB, 154645, 39483, 
					&Variable::Sub, &Variable::Value_i);
	TestArithmetic<float>(OP_F_PUSH, OP_F_SUB, 13418.491f, 9384.184f, 
					&Variable::Sub, &Variable::Value_f);
	TestArithmetic<double>(OP_D_PUSH, OP_D_SUB, 3418.491, 384.184, 
					&Variable::Sub, &Variable::Value_d);
	TestArithmetic<long>(OP_L_PUSH, OP_L_SUB, 341482738, 482184, 
					&Variable::Sub, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_SUB, 112, 102, 
					&Variable::Sub, &Variable::Value_c);
}

TEST (ExecutorTest, TestMul)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_MUL, 145, 33, 
					&Variable::Mul, &Variable::Value_i);
	TestArithmetic<float>(OP_F_PUSH, OP_F_MUL, 13.491f, 94.184f, 
					&Variable::Mul, &Variable::Value_f);
	TestArithmetic<double>(OP_D_PUSH, OP_D_MUL, 3834.491, 1284.184, 
					&Variable::Mul, &Variable::Value_d);
	TestArithmetic<long>(OP_L_PUSH, OP_L_MUL, 341, 484, 
					&Variable::Mul, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_MUL, 10, 13, 
					&Variable::Mul, &Variable::Value_c);
}

TEST (ExecutorTest, TestDiv)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_DIV, 1494, 33, 
					&Variable::Div, &Variable::Value_i);
	TestArithmetic<float>(OP_F_PUSH, OP_F_DIV, 38113.491f, 914.184f, 
					&Variable::Div, &Variable::Value_f);
	TestArithmetic<double>(OP_D_PUSH, OP_D_DIV, 3834.491, 1284.184, 
					&Variable::Div, &Variable::Value_d);
	TestArithmetic<long>(OP_L_PUSH, OP_L_DIV, 3274341, 484, 
					&Variable::Div, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_DIV, 220, 10, 
					&Variable::Div, &Variable::Value_c);
}

TEST (ExecutorTest, TestShl)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_SHL, 32, 5, 
					&Variable::Shl, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_SHL, 3341, 4, 
					&Variable::Shl, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_SHL, 1, 5, 
					&Variable::Shl, &Variable::Value_c);
}

TEST (ExecutorTest, TestShr)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_SHR, 32, 2, 
					&Variable::Shr, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_SHR, 3341, 4, 
					&Variable::Shr, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_SHR, 128, 3, 
					&Variable::Shr, &Variable::Value_c);
}

TEST (ExecutorTest, TestMod)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_MOD, 3832, 382, 
					&Variable::Mod, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_MOD, 3341, 244, 
					&Variable::Mod, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_MOD, 128, 14, 
					&Variable::Mod, &Variable::Value_c);
}

TEST (ExecutorTest, TestXor)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_XOR, 3832, 382, 
					&Variable::Xor, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_XOR, 3341, 244, 
					&Variable::Xor, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_XOR, 255, 14, 
					&Variable::Xor, &Variable::Value_c);
}

TEST (ExecutorTest, TestAnd)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_AND, 3832, 382, 
					&Variable::And, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_AND, 3341, 244,
					&Variable::And, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_AND, 255, 14, 
					&Variable::And, &Variable::Value_c);
}

TEST (ExecutorTest, TestOr)
{
	TestArithmetic<int>(OP_I_PUSH, OP_I_OR, 3832, 382, 
					&Variable::Or, &Variable::Value_i);
	TestArithmetic<long>(OP_L_PUSH, OP_L_OR, 3341, 244,
					&Variable::Or, &Variable::Value_l);
	TestArithmetic<char>(OP_C_PUSH, OP_C_OR, 255, 14, 
					&Variable::Or, &Variable::Value_c);
}
