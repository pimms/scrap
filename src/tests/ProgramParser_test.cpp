#include "ScrapTest.h"

/* The program in the file is created from something along the lines of this:
 *		
 *		class MainClass {
 *			int myInteger;
 *			bool myBool;
 *			static MainClass singleton;
 *
 *			float MyInstanceMethod(int intParam) { ... }
 *			static void Main() { ... }
 *		}
 */
#define SIMPLE_PROGRAM "testfiles/testprog"

/* Contains the exact same program as SIMPLE_PROGRAM, but with 
 * two trailing bytes.
 */
#define PROGRAM_WITH_TRAILING_BYTES "testfiles/testprog_trailing_bytes"



TEST (ProgramParserTest, TestParseProgram)
{
	ProgramParser parser;
	Program *program = NULL;

	ASSERT_NO_THROW({
		program = parser.ParseProgramFile(SIMPLE_PROGRAM);
		ASSERT_FALSE(program == NULL);
	});

	if (program) delete program;
}

TEST (ProgramParserTest, EnsureExceptionWhenFileHasTrailingBytes)
{
	ProgramParser parser;
	Program *program = NULL;

	ASSERT_ANY_THROW({
		program = parser.ParseProgramFile(PROGRAM_WITH_TRAILING_BYTES);
	});

	if (program) delete program;
}

TEST (ProgramParserTest, CheckCorrectParsing)
{
	ProgramParser parser;
	Program *program = parser.ParseProgramFile(SIMPLE_PROGRAM);

	const FunctionList *flist = program->GetFunctionList();

	Function *mainFunc = NULL;
	ASSERT_NO_THROW( mainFunc = flist->GetFunction(0)  );
	
	// Check the main function for validity
	vector<TypeDesc> args;
	FunctionAttributes attr = mainFunc->GetFunctionAttributes();

	ASSERT_EQ(attr.GetName(), "main");
	ASSERT_EQ(attr.GetReturnType().type, VarType::VOID);
	ASSERT_EQ(attr.GetReturnType().name, "");

	// Verify the arguments to MyInstanceMethod (int intParam)
	args = attr.GetArguments();
	ASSERT_EQ(args.size(), 1);
	ASSERT_EQ(args[0].type, VarType::INT);
	ASSERT_EQ(args[0].name, "intParam");

	// Ensure that the method bodies are read correctly
	const FunctionBody *body = mainFunc->GetFunctionBody();
	ASSERT_EQ(3, body->length);
	ASSERT_EQ(OP_POP, body->code[0]);		// pop
	ASSERT_EQ(OP_COPY, body->code[1]);		// copy
	ASSERT_EQ(OP_RETURN, body->code[2]);	// return

	delete program;
}

