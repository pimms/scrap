#pragma once

/***** Class Function *****
* Defines a function fragment. The eventual parameters
* of the function are parsed as expressions.
* FunctionCalls are ALWAYS 'owned' by an Expression.
*****/
#include <stdlib.h>
#include "tokens.h"
#include "expr.h"


class Expression;

class FunctionCall : public Fragment {
public:
					FunctionCall(Token *funcToken);
	void			ParseExpression(Tokens *tokens);

protected:
	Token::Type		mDelim;

	Token			*mFuncToken;
	list<Expression*>	mParams;
};
