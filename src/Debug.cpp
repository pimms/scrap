#include "Debug.h"
#include "FunctionInvocation.h"
#include "Variable.h"
#include "Function.h"
#include "Executor.h"
#include "IndexList.h"
#include "Bytecode.h"

#include <iostream>


namespace scrap {


/*
================
Debugger
================
*/
string Debugger::GetFunctionSignature(const FunctionInvocation *invoc) 
{
	const Function *method = invoc->GetFunction();
	string funcSig = method->GetFunctionAttributes().GetName();

	vector<TypeDesc> args = method->GetFunctionAttributes().GetArguments();

	// Add each of the arguments
	funcSig += "(";
	for (int i=0; i<args.size(); i++) {
		if (i) {
			funcSig += ", ";
		}
		
		funcSig += VarTypeToString(args[i].type).c_str();
	}

	funcSig += ")";
	return funcSig;
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


void CLISimpleDebugger::WillExecuteInstruction(const FunctionInvocation *invoc, const byte *instr) 
{
	const InstructionInfo info = g_instructionMap[instr[0]];	
	printf("will exec:  %s\n", info.name);
	TakeCommands(invoc);

	printf("exec %s\n", info.name);
}

void CLISimpleDebugger::DidInvokeNewFunction(const FunctionInvocation *invoc, const FunctionInvocation *oldInvoc) 
{
	printf("Function invoked: %s\n", GetFunctionSignature(invoc).c_str());
	TakeCommands(invoc);
}

void CLISimpleDebugger::DidReturn(const FunctionInvocation *invoc, const Variable *returnVariable) 
{
	printf("Function returned: %s\n", GetFunctionSignature(invoc).c_str());
	TakeCommands(invoc);
}



/*
================
CLISimpleDebugger Private 
================
*/
void CLISimpleDebugger::TakeCommands(const FunctionInvocation *invoc)
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


void CLISimpleDebugger::PrintStackContents(const FunctionInvocation *invoc)
{
	FunctionInvocation *i;
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
			}
			
			printf("]\n");
		}
	}
}




}
