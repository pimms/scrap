#pragma once

/***** Class Expression *****
* Binary tree-representation of expresions.
* All inputs are infix expressions, and are 
* stored in postfix format.
*****/
#include "fragment.h"
#include "func.h"

#include <map>
using namespace std;

class FunctionCall;

class Expression : public Fragment {
public:
	/***** Expression *****
	* @param isFunctionParam
	* 		 Is the expression a function parameter?
	* @param canBeNull
	* 		 Can the expression be empty? I.e., ";;;;;;".
	*****/
					Expression(bool isFunctionParam=false, bool canBeNull=true);
					~Expression();
	string			DbgGetString();

	void			ParseStatement(Tokens *tokens, Parser *parser);
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
	bool 			mCanBeNull;

	map<ExprTerm*,uint> mExprVars;

	void			BuildPostfix(Tokens *tokens, Parser *parser);
	int				OperatorPrecedence(Token *token);

	void			AllocateVariables(Opcode *opcode, Parser *parser);
	void			HandleFunctionCalls(Opcode *opcode, Parser *parser);
	void			BuildBytecodePostfix(Opcode *opcode, Parser *parser);

	void			AddOperator(Opcode *opcode, Token *token);
};
