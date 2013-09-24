#include "func.h"
#include "../common/stack.h"
#include "../common/scrapexcept.h"


FunctionCall::FunctionCall(Token *funcToken) {
	mFuncToken = funcToken;
}

void FunctionCall::ParseExpression(Tokens *tokens) {
	delete tokens->PopExpected(Token::PARANTH_BEG);

	if (tokens->PeekNext()->mType == Token::PARANTH_END) {
		delete tokens->PopNext();
	} else {
		while (tokens->PeekNext()->mType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->ParseExpression(tokens);
			mParams.push_back(expr);

			if (Token *t = tokens->PopIfExists(Token::COMMA)) {
				delete t;
			}
		}

		delete tokens->PopExpected(Token::PARANTH_END);
	}
}