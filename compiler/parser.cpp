#include "parser.h"
#include "expr.h"

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
		if (!BuildStatements()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build statements from tokens:\n");
		printf("%s\n", e.what());
	}

	try {
		if (!BuildIntermediates()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build intermediates:\n");
		printf("%s\n", e.what());
	}

	try {
		if (!BuildBytecode()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build bytecode:\n");
		printf("%s\n", e.what());
	}

	return true;
}

Opcode* Parser::GetOpcodes() {
	return NULL;
}


void Parser::PushScope() {
	mLScope.Push(new CompileScope);
}

void Parser::PopScope() {
	delete mLScope.Pop();
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
		throw VarNotDefined("Variable is not defined in the current scope: " + name);
	}

	return id;
}


bool Parser::BuildStatements() {
	while (mTokens->HasMore()) {
		Statement *statement = Statement::ParseStatement(mTokens);
		if (statement) {
			mStatements.push_back(statement);
		}
	}

	return true;
}

bool Parser::BuildIntermediates() {
	list<Statement*>::iterator it;
	for (it = mStatements.begin(); it != mStatements.end(); it++) {
		(*it)->ProvideIntermediates(mOpcode, this);
	}

	return true;
}

bool Parser::BuildBytecode() {

	return false;
}