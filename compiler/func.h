#pragma once

/***** Class FunctionCall *****
* Defines a function fragment. The eventual parameters
* of the function are parsed as expressions.
* FunctionCalls are ALWAYS 'owned' by an Expression.
*****/
#include <stdlib.h>
#include "fragment.h"
#include "tokens.h"

class Expression;

class FunctionCall : public Fragment {
public:
					FunctionCall(Token *funcToken);
	void			ParseStatement(Tokens *tokens);
	void			ProvideIntermediates(Opcode *opcode, Parser *parser);

	string			DbgGetString();

protected:
	Token::Type		mDelim;

	Token			*mFuncToken;

	list<Expression*>	mParams;

	void			HandleParameters(Opcode *opcode, Parser *parser);
};
