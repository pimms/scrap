#pragma once

#include "Scrap.h"

/* Environment
 *
 * The runtime environment of Scrap responsible for interpreting
 * and executing bytecode instructions.
 */
namespace scrap {

class Heap;
class Object;
class Class;

class Environment {
public:
	Environment();
	~Environment();

private:	
	Heap *_heap;

	void ExecutionLoop();
};

}
