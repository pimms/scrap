#pragma once

/***** Class Parser *****
* The primary interface for compiling a scrap-script
* to bytecode.
*****/

#include "tokens.h"
#include "../common/opcode.h"
#include "../common/scope.h"

class Parser {
public:
						Parser(string file, bool mainFile);
						~Parser();

	bool				ParseFile();
	bool 				CompileTokens();
	Opcode*				GetOpcodes();

private:
	/***** Static ID counters *****
	* The function-ID iterator is static to
	* ensure unique function IDs for all compiled
	* files.
	*****/
 	static uint			sFuncId;
	static uint			sGVarId;

	string				mFile;
	bool				mIsFileMain;
	Tokens				*mTokens;
	
	CompileScope		mGScope;
	Stack<CompileScope*>mLScope;

	void 				ParseReserved(Token *token);
};