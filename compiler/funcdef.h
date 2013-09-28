#pragma once


#include <stdlib.h>
#include "fragment.h"
#include "tokens.h"

class Expression;
class PositionReference;


/***** Class FunctionDefinition *****
* Defines the entry point for a function. This class handles
* the popping of parameters primarily.
*****/
class FunctionDefinition : public Fragment {
public:
	static bool			IsFunctionDefinition(Tokens *tokens);

	PositionReference*	GetPositionReference();

	void				ParseStatement(Tokens *tokens, Parser *parser);
	void				ProvideIntermediates(Opcode *opcode, Parser *parser);

protected:
	list<Token*>		mParams;
	PositionReference	*mPosRef;
};

