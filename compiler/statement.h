#pragma once

#include <stdlib.h>
#include "../common/codes.h"
#include "fragment.h"

class Expression;
class Tokens;
struct Token;

/***** Class Statement *****
* Abstract superclass for all statements.
*****/
class Statement : public Fragment {
protected:
						Statement();
			
public:
	static Statement*	CreateStatement(Tokens *tokens, Parser *parser);
};


/***** Class ScopeStatement ******
* Superclass of all statement-classes which
* by default pushes a scope. For-loops, while-loops
* and if/else-statements must know where the inner
* scope(s) end. This is done by inserting 'reference-objects'
* into the Statement-list handled by the Opcode object.
*****/
// TODO

/***** Class IfStatement *****
* 
*****/
// TODO


/***** Class AssignStatement *****
* A statement which may assign a new value to
* a variable. The variable may be allocated
* by the same statement.
*
* There are four cases falling into AssignStatement:
*	Allocation and assigninment:		var x = 1;
*	Allocation without assignment:		var x;
*	Reassignment:						x = 3;
*	Expression without assignment:		f(32);
* Do note that all assigning operators are supported (+=, -=, =, etc)
*****/
class AssignStatement : public Statement {
public:
	void			ParseStatement(Tokens *tokens, Parser *parser);
	void			ProvideIntermediates(Opcode *opcode, Parser *parser);

private:
	bool			mAlloc;
	Token			*mAssignee;
	Token			*mOperator;
	Expression		*mExpression;

	void			HandleOperator(Opcode *opcode, uint varId);
};


/***** Class ReturnStatement *****
* Handles returns from functions.
*****/
class ReturnStatement : public Statement {
public:
	void			ParseStatement(Tokens *tokens, Parser *parser);
	void			ProvideIntermediates(Opcode *opcode, Parser *parser);

private:
	Expression		*mExpression;
};