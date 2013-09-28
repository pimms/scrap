#pragma once

/***** Class Fragment *****
* 
*****/
#include <stdlib.h>
#include <string>

#include "../common/codes.h"
using namespace std;

class Opcode;
class Parser;
class Tokens;

class Fragment {
public:
	virtual void	ParseStatement(Tokens *tokens) = 0;
	virtual void	ProvideIntermediates(Opcode *opcode, Parser *parser) = 0;
	
	virtual string	DbgGetString();

protected:
	uint			RegisterVariable(Parser *parser, string name);
	uint			GetVariableId(Parser *parser, string name);

	void			AllocateVariable(Opcode *opcode, uint varId);
};
