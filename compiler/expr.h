#pragma once

/***** Class Expression *****
* Binary tree-representation of expresions.
* All inputs are infix expressions, and are 
* stored in postfix format.
*****/
#include "fragment.h"
#include "func.h"

class FunctionCall;

class Expression : public Fragment {
public:
					Expression(bool isFunctionParam = false);
					~Expression();
	string			DbgGetString();

	void			ParseStatement(Tokens *tokens);
	void			ProvideIntermediates(Opcode *opcode, Parser *parser);

private:
	struct ExprTerm {
		ExprTerm(FunctionCall *f) {
			mFunc = f;
			mToken = NULL;
		}

		ExprTerm(Token *t) {
			mToken = t;
			mFunc = NULL;
		}

		FunctionCall	*mFunc;
		Token			*mToken;
	};

	list<ExprTerm*>	mPostfix;
	bool			mIsParam;

	map<ExprTerm*,uint> mExprVars;

	void			BuildPostfix(Tokens *tokens);
	int				OperatorPrecedence(Token *token);

	void			AllocateVariables(Opcode *opcode, Parser *parser);
	void			HandleFunctionCalls(Opcode *opcode, Parser *parser);
	void			BuildBytecodePostfix(Opcode *opcode, Parser *parser);

	void			AddOperator(Opcode *opcode, Token *token);
};
