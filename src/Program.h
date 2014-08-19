#pragma once

#include "Scrap.h"

namespace scrap {

class Function;

/* Program
 * The highest level execution unit.
 */
class Program {
public:
	Program();
	~Program();

	void Execute();

private:
	Function *_mainFunction;
};

}
