#include "func.h"
#include "interop.h"
#include "expr.h"
#include "parser.h"

#include "../common/stack.h"
#include "../common/scrapexcept.h"

FunctionCall::FunctionCall(Token *funcToken) {
	mFuncToken = funcToken;
}

void FunctionCall::ParseStatement(Tokens *tokens) {
	delete tokens->PopExpected(Token::PARANTH_BEG);

	if (tokens->PeekNext()->mType == Token::PARANTH_END) {
		delete tokens->PopNext();
	} else {
		while (tokens->PeekNext()->mType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->ParseStatement(tokens);
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