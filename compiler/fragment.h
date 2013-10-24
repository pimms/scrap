#pragma once

/***** Class Fragment *****
* Fragments are created from a series of Tokens, and
* are provide IntermediateOperations for an Opcode-object.
*
***** ¡IMPORTANT! *****
* FRAGMENTS WHICH EXPECTS BRACKETS ARE EXPECTED TO ENSURE 
* THAT THE OPENING BRACKET EXISTS, BUT ARE REQUIRED TO 
* LEAVE THE BRACKET_BEG TOKEN IN THE TOKEN-STREAM.
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
	virtual void	ParseFragment(Tokens *tokens, Parser *parser) = 0;
	virtual void	ProvideIntermediates(Opcode *opcode, Parser *parser) = 0;
	
	virtual string	DbgGetString();

protected:
	uint			RegisterVariable(Parser *parser, string name);
	uint			GetVariableId(Parser *parser, string name);

	void			AllocateVariable(Opcode *opcode, uint varId);
};
