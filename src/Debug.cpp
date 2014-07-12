#include "Debug.h"
#include "MethodInvocation.h"
#include "Variable.h"
#include "Object.h"
#include "Method.h"
#include "Executor.h"
#include "Class.h"
#include "IndexList.h"

#include <iostream>


namespace scrap {


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
	
}

void CLISimpleDebugger::DidInvokeNewMethod(const MethodInvocation *invoc, const MethodInvocation *oldInvoc) 
{
	const Method *method = invoc->GetMethod();
	string methodName = method->GetMethodAttributes().GetName();

	const Class *cls = method->GetClass();
	string className = cls->GetClassName();

	vector<TypeDesc> args = method->GetMethodAttributes().GetArguments();

	printf("Method invoked: %s.%s", className.c_str(), methodName.c_str());

	// Print each of the arguments
	printf("(");
	for (int i=0; i<args.size(); i++) {
		if (i)
			printf(", ");
		
		if (args[i].type != VarType::OBJECT) {
			printf("%s", VarTypeToString(args[i].type).c_str());
		} else {
			const Class *c = invoc->GetClassList()->GetClass(args[i].classID);
			if (c != NULL) {
				printf("%s", c->GetClassName().c_str());
			} else {
				printf("<UnkownClass>");
			}
		}
	}
	printf(")");
	printf("\n");

	TakeCommands();
}

void CLISimpleDebugger::DidReturn(const MethodInvocation *invoc, const Variable *returnVariable) 
{

}



/*
================
CLISimpleDebugger Private 
================
*/
void CLISimpleDebugger::TakeCommands()
{
	string line;
	bool cont = true;

	do {
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
			}
		}

	} while (cont);
}


}
