#include "Environment.h"
#include "Heap.h"
#include "Object.h"
#include "Stack.h"
#include "Variable.h"
#include "Class.h"


namespace scrap {

Environment::Environment() 
{
	_heap = new Heap();
}

Environment::~Environment()
{
	_heap->KillOrphans();
	delete _heap;
}


void Environment::ExecutionLoop()
{
	
}

}
