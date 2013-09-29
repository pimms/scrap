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
#include "common/optostr.h"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("No file specified.\n");
		return 1;
	} else {
		string file = argv[1];
		
		Parser parser(file, true);
		
		if (parser.ParseFile()) {

			try {
				if (parser.CompileTokens()) {
					try {
						OpcodeText tostr;
						tostr.Parse(parser.GetOpcodes());
						
						Environment env(parser.GetOpcodes());
						env.Execute();
					} catch (exception &e) {
						printf("Runtime error:\n%s\n", e.what());
					}
				} else {
					printf("Compilation failed.\n");
				}
			} catch (exception &e) {
				printf("Compilation failed.\n%s\n", e.what());
			}
		}
	}
	
#	ifdef _WIN32
		_CrtDumpMemoryLeaks();
		getchar();
#	endif

	return 0;
}