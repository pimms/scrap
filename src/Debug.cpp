#include "Debug.h"
#include "MethodInvocation.h"
#include "Variable.h"
#include "Object.h"
#include "Method.h"
#include "Executor.h"
#include "Class.h"
#include "IndexList.h"
#include "Bytecode.h"

#include <iostream>


namespace scrap {


/*
================
Debugger
================
*/
string Debugger::GetMethodSignature(const MethodInvocation *invoc) 
{
	string retval = "";

	const Method *method = invoc->GetMethod();
	string methodName = method->GetMethodAttributes().GetName();

	const Class *cls = method->GetClass();
	string className = cls->GetClassName();

	vector<TypeDesc> args = method->GetMethodAttributes().GetArguments();

	retval += methodName + "." + className;

	// Add each of the arguments
	retval += "(";
	for (int i=0; i<args.size(); i++) {
		if (i)
			retval += ", ";
		
		if (args[i].type != VarType::OBJECT) {
			retval += VarTypeToString(args[i].type).c_str();
		} else {
			const Class *c = invoc->GetClassList()->GetClass(args[i].classID);
			if (c != NULL) {
				retval += c->GetClassName().c_str();
			} else {
				retval += "<UnkownClass>";
			}
		}
	}
	retval += ")";
	return retval;
}



/*
================
CLISimpleDebugger Public
================
*/
CLISimpleDebugger::CLISimpleDebugger() 
{

}

CLISimpleDebugger::~CLISimpleDebugger()
{

}


void CLISimpleDebugger::WillExecuteInstruction(const MethodInvocation *invoc, const byte *instr) 
{
	const InstructionInfo info = g_instructionMap[instr[0]];	
	printf("will exec:  %s\n", info.name);
	TakeCommands(invoc);

	printf("exec %s\n", info.name);
}

void CLISimpleDebugger::DidInvokeNewMethod(const MethodInvocation *invoc, const MethodInvocation *oldInvoc) 
{
	printf("Method invoked: %s\n", GetMethodSignature(invoc).c_str());
	TakeCommands(invoc);
}

void CLISimpleDebugger::DidReturn(const MethodInvocation *invoc, const Variable *returnVariable) 
{
	printf("Method returned: %s\n", GetMethodSignature(invoc).c_str());
	TakeCommands(invoc);
}



/*
================
CLISimpleDebugger Private 
================
*/
void CLISimpleDebugger::TakeCommands(const MethodInvocation *invoc)
{
	string line;
	bool cont = true;

	do {
		printf("$> ");
		std::getline(std::cin, line);
		
		if (line.length() == 0) {
			cont = false;
		} else if (line.length() == 1) {
			switch (std::toupper(line[0])) {
				case 'Q':
					cont = false;
					break;
				case 'N':
					cont = false;
					break;
				case 'S':
					PrintStackContents(invoc);
					break;
			}
		}

	} while (cont);
}


void CLISimpleDebugger::PrintStackContents(const MethodInvocation *invoc)
{
	MethodInvocation *i;
	const Stack *stack = invoc->GetStack();

	printf("%i elements\n", stack->Count());

	for (int i=stack->Count()-1; i>=0; i--) {
		const Variable *var = stack->Peek(i);
		if (var != NULL) {
			printf("[%s ", VarTypeToString(var->Type()).c_str());

			switch (var->Type()) {
				case VarType::INT:
					printf("{ %i }", var->Value_i());
					break;
				case VarType::LONG:
					printf("{ %li }", var->Value_l());
					break;
				case VarType::FLOAT:
					printf("{ %f }", var->Value_f());
					break;
				case VarType::DOUBLE:
					printf("{ %f }", var->Value_d());
					break;
				case VarType::VOID:
					printf("{ ... }");
					break;
				case VarType::BOOL:
					printf("{ %s }", (var->Value_b() ? "true" : "false"));
					break;
				case VarType::CHAR:
					printf("{ %c }", var->Value_c());
					break;
				case VarType::OBJECT:
					printf("%s { %i, ... }", var->Value_a()->GetClass()->GetClassName().c_str(),
											 var->Value_a()->RetainCount());
					break;
			}
			
			printf("]\n");
		}
	}
}




}
