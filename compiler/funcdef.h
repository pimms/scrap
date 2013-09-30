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

						FunctionDefinition();
	PositionReference*	GetPositionReference();

	void				ParseStatement(Tokens *tokens, Parser *parser);
	void				ProvideIntermediates(Opcode *opcode, Parser *parser);

	uint				GetId();

protected:
	list<Token*>		mParams;
	PositionReference	*mPosRef;
	uint				mFuncId;
};


/***** Class FunctionTail *****
* Inserted by the Parser object after an entire function has
* been fragmented. Inserts the final intermediates required
* for a function to behave properly, most notably the OP_RET.
*
* All FunctionTails have a PositionReference intermediate to
* inform the header-jump of the final function-body.
*****/
class FunctionTail : public Fragment {
public:
						FunctionTail();
	void				ParseStatement(Tokens *tokens, Parser *parser);
	void				ProvideIntermediates(Opcode *opcode, Parser *parser);
	PositionReference*	GetPositionReference();

private:
	PositionReference	*mPosRef;
};
