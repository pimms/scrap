#pragma once

#include <stdlib.h>
#include "tokens.h"
#include "expr.h"
#include "../common/opcode.h"

class Expression;

/***** Class Statement *****
* Abstract superclass for all statements.
*****/
class Statement : public Fragment {
protected:
						Statement();
			
public:
	static Statement*	ParseStatement(Tokens *tokens);
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
* A statement which assigns a new value to
* a variable. The variable may be allocated
* by the same statement.
*****/
class AssignStatement : public Statement {
public:
	void			ParseStatement(Tokens *tokens);
	void			ProvideIntermediates(Opcode *opcode, Parser *parser);

private:
	bool			mAlloc;
	Token			*mAssignee;
	Token			*mOperator;
	Expression		*mExpression;
};
