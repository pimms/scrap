#pragma once

#include "Scrap.h"

namespace scrap {

class Function;
class FunctionList;

/* Program
 * The highest level execution unit.
 */
class Program {
public:
	Program(FunctionList *funcList, unsigned mainFuncIndex);
	~Program();

	void Execute();

	FunctionList* GetFunctionList() const;
	Function* GetMainFunction() const;

private:
	FunctionList *_funcList;
	Function *_mainFunction;
};

}
