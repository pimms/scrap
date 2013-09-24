#include "parser.h"
#include "expr.h"

uint Parser::sFuncId = 0;
uint Parser::sGVarId = 0;


Parser::Parser(string file, bool mainFile) {
	mFile = file;
	mIsFileMain = mainFile;

	mTokens = new Tokens();
}

Parser::~Parser() {
	delete mTokens;
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
	printf("\nExpressions:\n");
	while (mTokens->HasMore()) {
		while (mTokens->HasMore()) {
			if (mTokens->PeekNext()->mToken == "=") {
				mTokens->PopNext();
				break;
			}
			mTokens->PopNext();
		}

		Expression expr;
		expr.ParseExpression(mTokens);
	}

	return false;
}

Opcode* Parser::GetOpcodes() {
	return NULL;
}


void Parser::ParseReserved(Token *token) {
	if (token->mToken == "var") {
		
	} else if (token->mToken == "for") {
		
	} else if (token->mToken == "class") {

	} else if (token->mToken == "if") {

	} else if (token->mToken == "else") {

	} else if (token->mToken == "while") {

	} else if (token->mToken == "func") {

	} else {
		
	}
}