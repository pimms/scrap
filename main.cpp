#ifdef _WIN32 
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "run/env.h"
#include "common/opcode.h"
#include "compiler/parser.h"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("No file specified.\n");
		return 1;
	} else {
		string file = argv[1];
		
		Parser parser(file, true);
		
		if (parser.ParseFile()) {
			printf("VALID SCRAP\n");
			if (parser.CompileTokens()) {
				Environment env(parser.GetOpcodes());
				env.Execute();
			}
		}
	}
	
#	ifdef _WIN32
		_CrtDumpMemoryLeaks();
		getchar();
#	endif

	return 0;
}