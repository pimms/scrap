#pragma once

/***** Class Parser *****
* The primary interface for compiling a scrap-script
* to bytecode.
*****/

#include "tokens.h"
#include "statement.h"
#include "../common/opcode.h"
#include "../common/scope.h"

class Statement;

class Parser {
public:
						Parser(string file, bool mainFile);
						~Parser();

	bool				ParseFile();
	bool 				CompileTokens();
	Opcode*				GetOpcodes();

	/***** Push / Pop scope *****
	* Modifies the current context of variable-scope.
	* This ensures that any requested variables in
	* fact does exist. 
	*****/
	void				PushScope();
	void				PopScope();

	void				PushNestedScope();
	void				PopNestedScope();

	uint				RegisterVariable(string name);
	uint				GetVariableId(string name);

	uint				RegisterFunction(string name);
	uint				GetFunctionId(string name);

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

	Opcode				*mOpcode;
	
	CompileScope		mGScope;
	Stack<CompileScope*>mLScope;

	list<Fragment*>		mFragments;

	map<string,uint>	mFuncIds;

	bool				BuildFragments();
	bool				BuildIntermediates();
	bool				BuildBytecode();

	void				BuildFunctionIntermediates();
	
};