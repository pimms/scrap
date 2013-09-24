#pragma once

/***** Class Fragment *****
* 
*****/
#include <stdlib.h>
#include "tokens.h"

class Fragment {
public:
	virtual void	ParseExpression(Tokens *tokens) = 0;

};
