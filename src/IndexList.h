#pragma once

#include "Scrap.h"


namespace scrap {

class Variable;
class Function;


/* Index Lists
 * Place commonly used containers for objects in these files.
 *
 * (After the rewrite removing OO from Scrap, only FunctionList 
 * remains, and IndexList.* served a bigger purpose back then)
 */


/* Function List
 * A list of function is 
 */
class FunctionList {
public:
	FunctionList();
	~FunctionList();

	void AddFunction(Function *function);
	Function* GetFunction(unsigned id) const;
	unsigned GetFunctionCount() const;

private:
	unsigned _inheritCount;
	vector<Function*> _functions;
};


}
