#include "func.h"
#include "interop.h"
#include "expr.h"
#include "parser.h"

#include "../common/stack.h"
#include "../common/scrapexcept.h"


/***** FunctionCall *****/
FunctionCall::FunctionCall(Token *funcToken) {
	mFuncToken = funcToken;
}

void FunctionCall::ParseStatement(Tokens *tokens, Parser *parser) {
	delete tokens->PopExpected(Token::PARANTH_BEG);

	if (tokens->PeekNext()->mType == Token::PARANTH_END) {
		delete tokens->PopNext();
	} else {
		while (tokens->PeekNext()->mType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->ParseStatement(tokens, parser);
			mParams.push_back(expr);

			if (Token *t = tokens->PopIfExists(Token::COMMA)) {
				delete t;
			}
		}

		delete tokens->PopExpected(Token::PARANTH_END);
	}
}

string FunctionCall::DbgGetString() {
	string ret = mFuncToken->mToken;

	ret += "(";

	list<Expression*>::iterator it;
	int i = 0;
	for (it = mParams.begin(); it != mParams.end(); it++) {
		if (i++) ret += ", ";
		ret += (*it)->DbgGetString();
	}

	ret += ") ";

	return ret;
}


void FunctionCall::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	HandleParameters(opcode, parser);

	uint funcId = parser->GetFunctionId(mFuncToken->mToken);

	opcode->AddInterop(new ByteOperation(OP_CALL));
	opcode->AddInterop(new DwordOperation(&funcId));
}

void FunctionCall::HandleParameters(Opcode *opcode, Parser *parser) {
	list<Expression*>::reverse_iterator it;
	for (it=mParams.rbegin(); it!=mParams.rend(); it++) {
		(*it)->ProvideIntermediates(opcode, parser);
	}
}



/***** FunctionDefinition *****/
bool FunctionDefinition::IsFunctionDefinition(Tokens *tokens) {
	return tokens->PeekNext()->mToken == "func";
}

FunctionDefinition::FunctionDefinition() {
	mFuncId = 0;
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
	mFuncId = parser->RegisterFunction(funcName->mToken);
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
}

uint FunctionDefinition::GetId() {
	return mFuncId;
}


/***** FunctionTail *****/
FunctionTail::FunctionTail() {
	mPosRef = new PositionReference();
}

void FunctionTail::ParseStatement(Tokens *tokens, Parser *parser) {
	// Nothing to do
}


void FunctionTail::ProvideIntermediates(Opcode *opcode, Parser *parser) {
	parser->PopScope();
	uint zero = 0;

	opcode->AddInterop(new ByteOperation(OP_RET));
	opcode->AddInterop(new DwordOperation(&zero));
	opcode->AddInterop(mPosRef);
}

PositionReference* FunctionTail::GetPositionReference() {
	return mPosRef;
}


/***** FunctionSignature *****/
FunctionSignature::FunctionSignature(string name, int paramCount) {
	mName = name;
	mParamCount = paramCount;
	mFunctionId = 0;
}

string FunctionSignature::GetName() {
	return mName;
}

int FunctionSignature::GetParamCount() {
	return mParamCount;
}

uint FunctionSignature::GetId() {
	return mFunctionId;
}

void FunctionSignature::SetId(uint id) {
	mFunctionId = id;
}