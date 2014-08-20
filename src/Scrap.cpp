#include "Scrap.h"

using namespace scrap;


const char *scrap_hello =
	"SCRAP\n"
	"=====\n\n"
	"Until things actually work, Scrap is limited to printing\n"
	"this message. However, it is a nice message.\n"
	"Development is for now focused on the test-suite and testable\n"
	"code. Around the time we are able to execute bytecode, this\n"
	"message will be replaced with something actually useful.\n\n"
	"Until then, enjoy this message :-)\n"
	;


#ifndef _SCRAP_TEST_
int main(int argc, char *argv[])
{
	printf("%s", scrap_hello);
	return 0;
}
#endif


namespace scrap {

string VarTypeToString(VarType t)
{
	switch (t) {
		case INT:
			return "int";
		case FLOAT:
			return "float";

		case DOUBLE:
			return "double";
		case LONG:
			return "long";
		case CHAR:
			return "char";
		case BOOL:
			return "bool";
		case VOID:
			return "void";
	}
	
	return "Undefined";
}

string AritOpToString(AritOp op)
{
	switch (op) {
		case ADD:
			return "add";
		case SUB:
			return "sub";
		case MUL:
			return "mul";
		case DIV:
			return "div";
		case SHR:
			return "shr";
		case SHL:
			return "shl";
		case MOD:
			return "mod";
		case XOR:
			return "xor";
		case AND:
			return "and";
		case OR:
			return "or";
	}

	return "undefined";
}


TypeDesc::TypeDesc() 
	:	type(VOID)
{ } 

TypeDesc::TypeDesc(VarType t)
	:	type(t)
{ }


TypeDesc::TypeDesc(VarType t, string name)
	:	type(t),
		name(name)
{ }

}
