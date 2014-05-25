#include "ScrapTest.h"

/* Testing Implementation Overview
 *
 * [ ] Pop
 * [ ] Copy
 * [ ] ArrayLength
 * [ ] ArrayLoad
 * [ ] Return
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
 * [ ] T Add
 * [ ] T Sub
 * [ ] T Mul
 * [ ] T Div
 * [ ] T Shl
 * [ ] T Shr
 * [ ] T Mod
 * [ ] T Xor
 * [ ] T And
 * [ ] T Or
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
void TestPushLiteral(byte instr, T val, T(Variable::*method)()const)
{
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
	ASSERT_EQ(val, (*var.*method)());
	
	delete var;
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
