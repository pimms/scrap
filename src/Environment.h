#pragma once

#include "Scrap.h"

/* Environment
 *
 * The runtime environment of Scrap responsible for interpreting
 * and executing bytecode instructions.
 */
namespace scrap {


class Environment {
public:
	Environment();
	~Environment();

private:	


	void ExecutionLoop();
};

}
