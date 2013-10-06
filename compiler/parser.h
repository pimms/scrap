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
class FunctionDefinition;
class PositionInquirer;
class FunctionSignature;

typedef list<Fragment*>::iterator FragmentIter;

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

	/***** IsInLocalScope *****
	* Is the Parser currently handing Tokens, Fragments 
	* or Intermediates in a local (function) scope?
	*****/
	bool				IsInLocalScope();

	void				PushNestedScope();
	void				PopNestedScope();

	uint				RegisterVariable(string name);
	uint				GetVariableId(string name);

	uint				RegisterFunction(FunctionSignature funcSign);
	uint				GetFunctionId(string name);
	FunctionSignature	GetFunctionSignature(string functionName);
	FunctionSignature	GetFunctionSignature(uint functionId);

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

	list<FunctionSignature> mFuncSigns;

	bool				BuildFragments();
	bool				BuildIntermediates();
	bool				BuildBytecode();

	void				AddFragment(Fragment *fragment);
	void				PushFragmentTail(FragmentIter tail);
	void				PopFragmentTail();

	/***** Data header functions *****
	* Adds position-inquirers to listen for the byte-position
	* of functions. 
	* TODO: string data
	*****/
	void				AddHeader();
	void				AddFunctionData(FunctionDefinition *funcDef);

	/***** Fragment Tail-iterators *****
	* Bytecode must be placed in the order:
	* [data header][jmp to global code][function definitions][global code]
	*
	* These iterators ensure that Fragments are placed in the correct
	* order.
	*****/
	Stack<FragmentIter> mFragmentTailStack;
	FragmentIter		mIterFuncdefEnd;
	InteropIter			mHeaderEnd;
	PositionInquirer	*mHeaderJump;
};