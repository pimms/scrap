#pragma once

/***** Class Parser *****
* The primary interface for compiling a scrap-script
* to bytecode.
*****/

#include "tokens.h"
#include "../common/opcode.h"

class Parser {
public:
						Parser(string file, bool mainFile);
						~Parser();
	bool				ParseFile();
	Opcode*				GetOpcodes();

private:
	string				mFile;
	bool				mIsFileMain;
	Tokens				*mTokens;
};