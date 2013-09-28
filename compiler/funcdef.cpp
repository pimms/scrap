#include "funcdef.h"
#include "interop.h"
#include "expr.h"
#include "parser.h"

#include "../common/stack.h"
#include "../common/scrapexcept.h"


// static
bool FunctionDefinition::IsFunctionDefinition(Tokens *tokens) {
	return tokens->PeekNext()->mToken == "func";
}



PositionReference* FunctionDefinition::GetPositionReference() {
	return mPosRef;
}


void FunctionDefinition::ParseStatement(Tokens *tokens, Parser *parser) {
	// Ensure that this really is a function
	Token *token = tokens->PopExpected(Token::RESERVED);
	if (token->mToken != "func") {
		throw InvalidTokenException("Expected 'func', got: " + token->mToken);
	}
	delete token;

	// Register the function
	Token *funcName = tokens->PopExpected(Token::VARFUNC);
	parser->RegisterFunction(funcName->mToken);
	delete funcName;

	// Delete the opening parantheses
	delete tokens->PopExpected(Token::PARANTH_BEG);

	// Snag those pesky parameters
	while (tokens->PeekNext()->mToken == "var") {
		delete tokens->PopExpected(Token::RESERVED);

		Token *param = tokens->PopExpected(Token::VARFUNC);
		mParams.push_back(param);

		Token *comma = tokens->PopIfExists(Token::COMMA);
		if (comma) {
			delete comma;
		}
	}

	delete tokens->PopExpected(Token::PARANTH_END);

	if (tokens->PeekNext()->mType != Token::BRACKET_BEG) {
		throw InvalidTokenException("Expected {");
	}

	mPosRef = new PositionReference();
}

void FunctionDefinition::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	parser->PushScope();
	opcode->AddInterop(mPosRef);

	for (TokenIter it = mParams.begin(); it != mParams.end(); it++) {
		uint varId = parser->RegisterVariable((*it)->mToken);

		// Allocate and assign the variable yo
		AllocateVariable(opcode, varId);
		opcode->AddInterop(new ByteOperation(OP_POPMOV));
		opcode->AddInterop(new DwordOperation(&varId));
	}
	
	// Push the tail
	InteropIter tail = opcode->AddInterop(new FunctionTail);
	opcode->PushTail(tail);
}