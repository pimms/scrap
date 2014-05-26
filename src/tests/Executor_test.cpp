#include "ScrapTest.h"

/* Testing Implementation Overview
 *
 * [X] Pop
 * [X] Copy
 * [ ] ArrayLength
 * [ ] ArrayLoad
 * [X] Return
 * [X] T Load
 * [ ] T Return
 * [X] T Store
 * [X] T Push
 * [ ] T NewArray
 * [ ] T ARelease
 * [ ] T ALoad
 * [ ] T AStore
 * [X] A2B
 * [X] F2T
 * [X] D2T
 * [X] L2T
 * [X] I2T
 * [X] C2T
 * [X] B2T
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
 * [X] New
 * [X] Retain
 * [X] Release
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


/* The test program is a manual compilation of a program similar to this:
 *		class MainClass {					// ID = 2
 *			static MainClass singleton;		// ID = 0
 *			Foo foo;						// ID = 0
 *			Bar bar;						// ID = 1
 *			
 *			static void Main(); 			// ID = 0
 *		}
 *
 *		class Foo {							// ID = 0
 *			static bool flag;				// ID = 0
 *			int num;						// ID = 0
 *			
 *			static void TrueFlag() {		// ID = 0
 *				flag = true;
 *			}
 *
 *			virtual int GetNum() {			// ID = 0
 *				return 10;
 *			}
 *		}
 *
 *		class Bar extends Foo {				// ID = 1
 *			virtual int GetNum() {			// ID = 0
 *				return 15;
 *			}
 *
 *			void SetNum() {					// ID = 1
 *				num = 13;
 *			}
 *		}
 *
 * The bytecode-length and bytecode of the "void Main()" method is not
 * included in the file and must be added before attempting to execute
 * the program.
 */
#define PROGRAM_BASE_FILE "testfiles/testprog_base"

/* To read the program file successfully, this class overrides 
 * ProgramParser. The method MainClass::Main(), is not read at all
 * but returns the pre-defined program defined in the test bodies. 
 * The remaining methods are read as normal.
 *
 * This class will only work properly when reading the test-program 
 * base file. 
 */
class TestProgramCreator : public ProgramParser {
public:
	TestProgramCreator(MethodBody body) {
		_body = body;
		_methodsRead = 0;
	}	

	MethodBody ReadMethodBody() {
		if (_methodsRead++ == 4)
			return _body;
		return ProgramParser::ReadMethodBody();
	}

private:
	MethodBody _body;
	int _methodsRead;
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
	Class *mainClass = clist->GetClass(2);
	Method *mainMethod = mainClass->GetStaticMethod(0);

	MethodInvocation invocation(heap, mainMethod, mainClass, NULL);
	invocation.SetClassList(clist);
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
 *		instr			The push instruction
 *		val				The value to be pushed
 *		valmethod		The type Variable-value returning the T-value
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

/* Tests an arithmetic operation. 
 * 		pushInstr		The instruction which pushes a literal
 * 		aritInstr 		The arithmetic bytecode instruction
 * 		val1 			The first value
 * 		val2 			The second value
 * 		opmethod 		The Variable-method corresponding to aritInstr
 * 		valmethod		The Variable-method returning the value
 */
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

/* Tests conversion between the types "FROM" and "TO".
 * 		pushInstr		The instruction pushing a value of type "FROM"
 * 		aritInstr		The conversion byte instruction
 * 		val				The input value to be converted
 * 		expectedOutput	The expected output of the conversion
 * 		expectedType	The expected VarType of the converted variable
 * 		valmethod		The Variable-method return the FROM-value
 */
template<typename FROM, typename TO>
void TestConversion(byte pushInstr, byte convInstr, FROM val, 
					TO expectedOutput, VarType expectedType,
					TO(Variable::*valmethod)()const)
{
	/* Execute the program:
	 * 		<pushInstr> <val>
	 * 		<convInstr>
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	Variable *var = NULL;
	MethodBody body;

	body.length = 2 + sizeof(FROM);
	body.code = new byte[body.length];
	body.code[0] = pushInstr;
	memcpy(body.code+1, &val, sizeof(FROM));
	body.code[1+sizeof(FROM)] = convInstr;

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 1);
	var = stack->Pop();
	
	ASSERT_EQ(var->Type(), expectedType);
	ASSERT_NEAR((*var.*valmethod)(), expectedOutput, 0.0001);

	delete var;
	delete stack;
	delete program;
}

/* Pushes a variable, stores it in a register and pops it back onto the stack.
 * Unfortunately, getting access directly to the registers is hard as the
 * Executor class managing the registers is managed by the MethodInvocation,
 * so testing that storing and loading works simultaneously is a good 
 * secondary solution.
 * 		pushInstr		The instruction pushing a value
 * 		storeInstr		Instruction storing a value in a register
 * 		loadInstr		Instruction loading a value from a register
 * 		val				The value of the variable to be jellied around
 */
template<typename T>
void TestLoadAndStore(byte pushInstr, byte storeInstr, byte loadInstr, T val)
{
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	Variable *var = NULL;
	MethodBody body;

	
	// Tpush <val>
	// Tstore N
	for (int i=0; i<NUM_REGISTERS; i++) {
		body.length = 3 + sizeof(T);
		body.code = new byte[body.length];
		body.code[0] = pushInstr;
		memcpy(body.code+1, &val, sizeof(T));
		body.code[1+sizeof(T)] = storeInstr;
		body.code[2+sizeof(T)] = (byte)i;

		program = CreateProgram(body);
		stack = ExecuteProgram(program, &heap);

		ASSERT_EQ(stack->Count(), 0);

		delete stack; 
		delete program;
	}

	// Tpush<val>
	// Tstore N
	// Tload N
	for (int i=0; i<NUM_REGISTERS; i++) {
		body.length = 5 + sizeof(T);
		body.code = new byte[body.length];
		body.code[0] = pushInstr;
		memcpy(body.code+1, &val, sizeof(T));
		body.code[1+sizeof(T)] = storeInstr;
		body.code[2+sizeof(T)] = (byte)i;
		body.code[3+sizeof(T)] = loadInstr;
		body.code[4+sizeof(T)] = (byte)i;

		program = CreateProgram(body);
		stack = ExecuteProgram(program, &heap);

		ASSERT_EQ(stack->Count(), 1);
		var = stack->Pop();

		delete var;
		delete stack;
		delete program;
	}
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

TEST (ExecutorTest, TestIntConversion)
{
	TestConversion<int,float>(OP_I_PUSH, OP_I2F, 45, 45.f, 
							VarType::f, &Variable::Value_f);
	TestConversion<int,double>(OP_I_PUSH, OP_I2D, 45, 45.0, 
							VarType::d, &Variable::Value_d);
	TestConversion<int,long>(OP_I_PUSH, OP_I2L, 45, 45, 
							VarType::l, &Variable::Value_l);
	TestConversion<int,char>(OP_I_PUSH, OP_I2C, 45, 45, 
							VarType::c, &Variable::Value_c);

	TestConversion<int,bool>(OP_I_PUSH, OP_I2B, 45, true,
							VarType::b, &Variable::Value_b);
	TestConversion<int,bool>(OP_I_PUSH, OP_I2B, 0, false,
							VarType::b, &Variable::Value_b);
	TestConversion<int,bool>(OP_I_PUSH, OP_I2B, -45, true,
							VarType::b, &Variable::Value_b);
}

TEST (ExecutorTest, TestFloatConversion)
{
	TestConversion<float,int>(OP_F_PUSH, OP_F2I, 45.9f, 45, 
							VarType::i, &Variable::Value_i);
	TestConversion<float,double>(OP_F_PUSH, OP_F2D, 45.9f, 45.9, 
							VarType::d, &Variable::Value_d);
	TestConversion<float,long>(OP_F_PUSH, OP_F2L, 45.9, 45, 
							VarType::l, &Variable::Value_l);
	TestConversion<float,char>(OP_F_PUSH, OP_F2C, 45.9, 45, 
							VarType::c, &Variable::Value_c);

	TestConversion<float,bool>(OP_F_PUSH, OP_F2B, 45.9, true,
							VarType::b, &Variable::Value_b);
	TestConversion<float,bool>(OP_F_PUSH, OP_F2B, 0.f, false,
							VarType::b, &Variable::Value_b);
	TestConversion<float,bool>(OP_F_PUSH, OP_F2B, -45.f, true,
							VarType::b, &Variable::Value_b);
}

TEST (ExecutorTest, TestDoubleConversion)
{
	TestConversion<double,int>(OP_D_PUSH, OP_D2I, 45.9, 45, 
							VarType::i, &Variable::Value_i);
	TestConversion<double,float>(OP_D_PUSH, OP_D2F, 45.9f, 45.9f, 
							VarType::f, &Variable::Value_f);
	TestConversion<double,long>(OP_D_PUSH, OP_D2L, 45.9, 45, 
							VarType::l, &Variable::Value_l);
	TestConversion<double,char>(OP_D_PUSH, OP_D2C, 45.9, 45, 
							VarType::c, &Variable::Value_c);
	
	TestConversion<double,bool>(OP_D_PUSH, OP_D2B, 45.9, true,
							VarType::b, &Variable::Value_b);
	TestConversion<double,bool>(OP_D_PUSH, OP_D2B, 0.0, false,
							VarType::b, &Variable::Value_b);
	TestConversion<double,bool>(OP_D_PUSH, OP_D2B, -45.f, true,
							VarType::b, &Variable::Value_b);
}

TEST (ExecutorTest, TestLongConversion)
{
	TestConversion<long,float>(OP_L_PUSH, OP_L2F, 45, 45.f, 
							VarType::f, &Variable::Value_f);
	TestConversion<long,double>(OP_L_PUSH, OP_L2D, 45, 45.0, 
							VarType::d, &Variable::Value_d);
	TestConversion<long,int>(OP_L_PUSH, OP_L2I, 45, 45, 
							VarType::i, &Variable::Value_i);
	TestConversion<long,char>(OP_L_PUSH, OP_L2C, 45, 45, 
							VarType::c, &Variable::Value_c);

	TestConversion<long,bool>(OP_L_PUSH, OP_L2B, 45, true,
							VarType::b, &Variable::Value_b);
	TestConversion<long,bool>(OP_L_PUSH, OP_L2B, 0, false,
							VarType::b, &Variable::Value_b);
	TestConversion<long,bool>(OP_L_PUSH, OP_L2B, -45, true,
							VarType::b, &Variable::Value_b);
}

TEST (ExecutorTest, TestCharConversion)
{
	TestConversion<char,float>(OP_C_PUSH, OP_C2F, 45, 45.f, 
							VarType::f, &Variable::Value_f);
	TestConversion<char,double>(OP_C_PUSH, OP_C2D, 45, 45.0, 
							VarType::d, &Variable::Value_d);
	TestConversion<char,int>(OP_C_PUSH, OP_C2I, 45, 45, 
							VarType::i, &Variable::Value_i);
	TestConversion<char,long>(OP_C_PUSH, OP_C2L, 45, 45, 
							VarType::l, &Variable::Value_l);

	TestConversion<char,bool>(OP_C_PUSH, OP_C2B, 45, true,
							VarType::b, &Variable::Value_b);
	TestConversion<char,bool>(OP_C_PUSH, OP_C2B, 0, false,
							VarType::b, &Variable::Value_b);
	TestConversion<char,bool>(OP_C_PUSH, OP_C2B, -45, true,
							VarType::b, &Variable::Value_b);
}

TEST (ExecutorTest, TestBoolConversion)
{
	TestConversion<bool,float>(OP_B_PUSH, OP_B2F, true, 1.f, 
							VarType::f, &Variable::Value_f);
	TestConversion<bool,double>(OP_B_PUSH, OP_B2D, true, 1.0, 
							VarType::d, &Variable::Value_d);
	TestConversion<bool,int>(OP_B_PUSH, OP_B2I, true, 1, 
							VarType::i, &Variable::Value_i);
	TestConversion<bool,long>(OP_B_PUSH, OP_B2L, true, 1, 
							VarType::l, &Variable::Value_l);
	TestConversion<bool,char>(OP_B_PUSH, OP_B2C, true, 1, 
							VarType::c, &Variable::Value_c);

	TestConversion<bool,float>(OP_B_PUSH, OP_B2F, false, 0.f, 
							VarType::f, &Variable::Value_f);
	TestConversion<bool,double>(OP_B_PUSH, OP_B2D, false, 0.0, 
							VarType::d, &Variable::Value_d);
	TestConversion<bool,int>(OP_B_PUSH, OP_B2I, false, 0, 
							VarType::i, &Variable::Value_i);
	TestConversion<bool,long>(OP_B_PUSH, OP_B2L, false, 0, 
							VarType::l, &Variable::Value_l);
	TestConversion<bool,char>(OP_B_PUSH, OP_B2C, false, 0, 
							VarType::c, &Variable::Value_c);
}

TEST (ExecutorTest, TestLoadAndStore)
{
	TestLoadAndStore<int>(OP_I_PUSH, OP_I_STORE, OP_I_LOAD, 45958);
	TestLoadAndStore<float>(OP_F_PUSH, OP_F_STORE, OP_F_LOAD, 45958.f);
	TestLoadAndStore<double>(OP_D_PUSH, OP_D_STORE, OP_D_LOAD, 45958.0);
	TestLoadAndStore<long>(OP_L_PUSH, OP_L_STORE, OP_L_LOAD, 45958);
	TestLoadAndStore<bool>(OP_B_PUSH, OP_B_STORE, OP_B_LOAD, true);
	TestLoadAndStore<char>(OP_C_PUSH, OP_C_STORE, OP_C_LOAD, 243);
}

TEST (ExecutorTest, TestNew)
{
	/* After the new-instruction, the object should exist on the stack as a 
	 * Variable and on the Heap.
	 * 		new 2
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	unsigned classID = 2;

	body.length = 1 + sizeof(classID);
	body.code = new byte[body.length];
	body.code[0] = OP_NEW;
	memcpy(body.code+1, &classID, sizeof(unsigned));

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 1);
	Variable *var = stack->Pop();
	ASSERT_EQ(var->Type(), VarType::a);

	Object *obj = var->Value_a();
	ASSERT_EQ(obj->GetClass()->GetClassName(), "MainClass");
	ASSERT_EQ(obj->GetClass()->GetClassID(), 2);

	ASSERT_EQ(heap.Size(), 1);

	obj->Release();
	heap.KillOrphans();
	
	delete var;
	delete stack;
	delete program;
}

TEST (ExecutorTest, TestRetain)
{
	/* After retaining the object, a call to Heap::KillOrphans() should NOT 
	 * delete the object. OP_RETAIN pops the object from the stack.
	 * 		new 0
	 * 		retain
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	unsigned classID = 0;

	body.length = 2 + sizeof(classID);
	body.code = new byte[body.length];
	body.code[0] = OP_NEW;
	memcpy(body.code+1, &classID, sizeof(unsigned));
	body.code[1+sizeof(unsigned)] = OP_RETAIN;

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 0);
	ASSERT_EQ(heap.Size(), 1);

	heap.GetObject(0)->Release();
	heap.KillOrphans();
	ASSERT_EQ(heap.Size(), 1);

	heap.GetObject(0)->Release();
	heap.KillOrphans();
	ASSERT_EQ(heap.Size(), 0);
	
	delete stack;
	delete program;
}

TEST (ExecutorTest, TestRelease)
{
	/* After releasing the object, the heap-instance should cleared after a
	 * call to Heap::KillOrphans().
	 * 		new 0
	 * 		release
	 */
	Stack *stack = NULL;
	Heap heap;
	Program *program = NULL;
	MethodBody body;
	unsigned classID = 0;

	body.length = 2 + sizeof(classID);
	body.code = new byte[body.length];
	body.code[0] = OP_NEW;
	memcpy(body.code+1, &classID, sizeof(unsigned));
	body.code[1+sizeof(unsigned)] = OP_RELEASE;

	program = CreateProgram(body);
	stack = ExecuteProgram(program, &heap);

	ASSERT_EQ(stack->Count(), 0);
	ASSERT_EQ(heap.Size(), 1);

	heap.KillOrphans();
	ASSERT_EQ(heap.Size(), 0);

	delete stack;
	delete program;
}

