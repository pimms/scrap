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

	ClassList *clist = program->GetClassList();

	Class *mainClass = NULL;
	ASSERT_NO_THROW( mainClass = clist->GetClass(0)  );
	ASSERT_EQ(mainClass->GetClassName(), "MainClass");
	
	const Field *myInt = NULL;  
	const Field *myBool = NULL; 
	const Field *singleton = NULL;
	ASSERT_NO_THROW(
		myInt = mainClass->GetField(0);
		myBool = mainClass->GetField(1);
		singleton = mainClass->GetStaticField(0);
	);

	ASSERT_EQ(myInt->typeDesc.name, "myInteger");
	ASSERT_EQ(myInt->typeDesc.type, VarType::INT);
	ASSERT_EQ(myInt->typeDesc.classID, ID_UNDEFINED);

	ASSERT_EQ(myBool->typeDesc.name, "myBool");
	ASSERT_EQ(myBool->typeDesc.type, VarType::BOOL);
	ASSERT_EQ(myBool->typeDesc.classID, ID_UNDEFINED);

	ASSERT_EQ(singleton->typeDesc.name, "singleton");
	ASSERT_EQ(singleton->typeDesc.type, VarType::OBJECT);
	ASSERT_EQ(singleton->typeDesc.classID, 0);


	// Check the methods for validity
	vector<TypeDesc> args;
	Method *imethod = mainClass->GetMethod(0);
	Method *smethod = mainClass->GetStaticMethod(0);
	MethodAttributes iattr = imethod->GetMethodAttributes();
	MethodAttributes sattr = smethod->GetMethodAttributes();

	ASSERT_EQ(iattr.GetName(), "MyInstanceMethod");
	ASSERT_EQ(iattr.GetReturnType().type, VarType::FLOAT);
	ASSERT_EQ(iattr.GetReturnType().name, "");
	ASSERT_EQ(iattr.GetReturnType().classID, ID_UNDEFINED);

	ASSERT_EQ(smethod->GetMethodAttributes().GetName(), "Main");
	ASSERT_EQ(sattr.GetReturnType().type, VarType::VOID);
	ASSERT_EQ(sattr.GetReturnType().name, "");
	ASSERT_EQ(sattr.GetReturnType().classID, ID_UNDEFINED);

	// Verify the arguments to MyInstanceMethod (int intParam)
	args = iattr.GetArguments();
	ASSERT_EQ(args.size(), 1);
	ASSERT_EQ(args[0].type, VarType::INT);
	ASSERT_EQ(args[0].name, "intParam");
	ASSERT_EQ(args[0].classID, ID_UNDEFINED);

	args = sattr.GetArguments();
	ASSERT_EQ(args.size(), 0);

	// Ensure that the method bodies are read correctly
	const MethodBody *body = imethod->GetMethodBody();
	ASSERT_EQ(11, body->length);
	ASSERT_EQ(0x00, body->code[0]);		// pop
	ASSERT_EQ(0x01, body->code[1]);		// copy
	ASSERT_EQ(0x02, body->code[2]);		// arraylength
	ASSERT_EQ(0x03, body->code[3]);		// arrayload $0
	ASSERT_EQ(0x00, body->code[4]);
	ASSERT_EQ(0x33, body->code[5]);		// fpush 4.3
	ASSERT_EQ(0x31, body->code[10]);	// freturn

	float fpush = *(float*)(body->code + 6);
	ASSERT_TRUE( abs(4.3 - fpush) < 0.0000001 );

	body = smethod->GetMethodBody();
	ASSERT_EQ(1, body->length);
	ASSERT_EQ(0x04, body->code[0]);		// return

	delete program;
}

