#include "parser.h"

uint Parser::sFuncId = 0;


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