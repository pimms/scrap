#include "Scrap.h"

using namespace scrap;

string VarTypeToString(VarType t)
{
	switch (t) {
		case OBJECT:
			return "Object";
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


int main(int argc, char *argv[]) 
{
	try {
		THROW(InvalidTypeException, "hey hey");
	} catch (GenericException ex) {
		printf("Exception caught:\n%s\n", ex.msg.c_str());
	}
}

