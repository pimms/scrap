#include "parser.h"
#include "expr.h"
#include "funcdef.h"
#include "interop.h"

uint Parser::sFuncId = 0;
uint Parser::sGVarId = 0;


Parser::Parser(string file, bool mainFile) {
	mFile = file;
	mIsFileMain = mainFile;

	mTokens = new Tokens();
	mOpcode = new Opcode();
}

Parser::~Parser() {
	delete mTokens;
	delete mOpcode;
}


bool Parser::ParseFile() {
	try {
		mTokens->BuildTokens(mFile);
	} catch (exception e) {
		printf("Failed to parse file '%s':\n", mFile.c_str());
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

bool Parser::CompileTokens() {
	try {
		if (!BuildFragments()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build fragments from tokens:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildIntermediates()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build intermediates:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildBytecode()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build bytecode:\n");
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

Opcode* Parser::GetOpcodes() {
	return mOpcode;
}


void Parser::PushScope() {
	mLScope.Push(new CompileScope);
}

void Parser::PopScope() {
	delete mLScope.Pop();
}

bool Parser::IsInLocalScope() {
	return mLScope.Size() > 0;
}


void Parser::PushNestedScope() {
	if (mLScope.Size()) {
		mLScope.Peek()->PushNestedScope();
	} else {
		mGScope.PushNestedScope();
	}
}

void Parser::PopNestedScope() {
	if (mLScope.Size()) {
		mLScope.Peek()->PopNestedScope();
	} else {
		mGScope.PopNestedScope();
	}
}


uint Parser::RegisterVariable(string name) {
	// TODO:
	// Ensure that the variable isn't already defined
	CompileScope *scope = NULL;
	uint id = 0;

	if (mLScope.Size()) {
		scope = mLScope.Peek();
		id = VAR_LOCAL | (mLScope.Peek()->GetVarCount());
	} else {
		scope = &mGScope;
		id = VAR_GLOBAL | (++sGVarId);
	}

	string *n = new string;
	*n = name;

	scope->AddItem(id, n);
	return id;
}

uint Parser::GetVariableId(string name) {
	if (!name.length()) {
		throw runtime_error("Are you out of your fkn mind?");
	}

	uint id = 0;

	if (mLScope.Size()) {
		id = mLScope.Peek()->GetItemId(&name);
		if (id) { 
			return id;
		}
	}

	id = mGScope.GetItemId(&name);
	
	if (id == 0) {
		throw VarNotDefinedException("Variable is not defined in the current scope: " + name);
	}

	return id;
}



uint Parser::RegisterFunction(string name) {
	if (mFuncIds.count(name)) {
		throw FuncAlreadyDefinedException("Function " + name + " is already defined");
	}

	mFuncIds[name] = ++sFuncId;
	return sFuncId;
}

uint Parser::GetFunctionId(string name) {
	if (!mFuncIds.count(name)) {
		throw FuncNotDefinedException("Function is undefined: " + name);
	}

	return mFuncIds[name];
}


bool Parser::BuildFragments() {
	PushFragmentTail(mFragments.end());
	AddHeader();
	mIterFuncdefEnd = mFragments.insert(mFragments.end(), NULL);

	int stackDepth = 0;
	bool inFunction = false;

	while (mTokens->HasMore()) {
		Token *next = mTokens->PeekNext();

		Statement *statement = Statement::CreateStatement(mTokens, this);
		if (statement) {
			AddFragment(statement);
		} else if (FunctionDefinition::IsFunctionDefinition(mTokens)) {
			FunctionDefinition *fdef = new FunctionDefinition();
			fdef->ParseStatement(mTokens, this);


			PushFragmentTail(mIterFuncdefEnd);
			AddFragment(fdef);
			AddFunctionData(fdef);

			delete mTokens->PopExpected(Token::BRACKET_BEG);

			if (inFunction) {
				throw SyntaxErrorException("Funexception");
			}

			stackDepth++;
			inFunction = true;
			PushScope();

		} else if (next->mType == Token::BRACKET_BEG) {
			stackDepth++;
			PushNestedScope();
			delete mTokens->PopNext();
		} else if (next->mType == Token::BRACKET_END) {
			if (--stackDepth == 0) {
				if (inFunction) {
					PopScope();
					inFunction = false;

					FunctionTail *tail = new FunctionTail();
					AddFragment(tail);
					PopFragmentTail();

					tail->GetPositionReference()->AddInquirer(mHeaderJump);
				} else {
					throw InvalidTokenException("Unexpected }");
				}
			} else {
				PopNestedScope();
			}
			delete mTokens->PopNext();
		} else {
			throw InvalidTokenException("Unexpected '" + next->mToken + "'!");
		}
	}

	return true;
}

bool Parser::BuildIntermediates() {
	list<Fragment*>::iterator it;
	for (it = mFragments.begin(); it != mFragments.end(); it++) {
		if (!*it) continue;
		(*it)->ProvideIntermediates(mOpcode, this);
	}

	// Add a final exit-statement
	uint zero = 0;
	mOpcode->AddInterop(new ByteOperation(OP_EXIT));
	mOpcode->AddInterop(new DwordOperation(&zero));

	return true;
}

bool Parser::BuildBytecode() {
	return mOpcode->BuildBytecodeFromIntermediates();
}


void Parser::AddFragment(Fragment *fragment) {
	mFragments.insert(mFragmentTailStack.Peek(), fragment);
}

void Parser::PushFragmentTail(FragmentIter iter) {
	mFragmentTailStack.Push(iter);
}

void Parser::PopFragmentTail() {
	mFragmentTailStack.Pop();
}


void Parser::AddHeader() {
	if (mOpcode->Length() != 0) {
		throw InternalErrorException();
	}
	
	mOpcode->AddInterop(new ByteOperation(OP_DATA_BEGIN));
	mHeaderEnd = mOpcode->AddInterop(new ByteOperation(OP_DATA_END));

	// Add the header-jump
	mHeaderJump = new PositionInquirer();
	mOpcode->AddInterop(new ByteOperation(OP_JMP));
	mOpcode->AddInterop(mHeaderJump);
}	

void Parser::AddFunctionData(FunctionDefinition *funcDef) {
	mOpcode->PushTail(mHeaderEnd);

	uint funcId = funcDef->GetId();

	mOpcode->AddInterop(new ByteOperation(OP_DATA_FUNC));
	mOpcode->AddInterop(new DwordOperation(&funcId));

	PositionInquirer *posInq = new PositionInquirer();
	funcDef->GetPositionReference()->AddInquirer(posInq);
	mOpcode->AddInterop(posInq);

	mOpcode->PopTail();
}
