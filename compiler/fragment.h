#pragma once

/***** Class Fragment *****
* 
*****/
#include <stdlib.h>
#include "tokens.h"
#include "parser.h"
#include "interop.h"
#include "../common/opcode.h"


class Fragment {
public:
	virtual void	ParseStatement(Tokens *tokens) = 0;
	virtual void	ProvideIntermediates(Opcode *opcode, Parser *parser) = 0;
	
	virtual string	DbgGetString();

protected:
	uint			RegisterVariable(Parser *parser, string name);
	uint			GetVariableId(Parser *parser, string name);
};
