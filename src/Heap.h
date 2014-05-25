#pragma once

#include "Scrap.h"

namespace scrap{

class Object;
class Class;

/* Heap
 * The heap is responsible for creating and managing heap allocated
 * objects. Objects can be referenced by many different sources, and
 * the relationship graph quickly becomes convoluted. The Heap keeps
 * track of all living Objects and deletes them whenever their reference
 * counter reaches 0. 
 */
class Heap {
public:
	Heap();
	~Heap();

	Object* CreateObject(const Class *c);

	// Delete objects with no references
	void KillOrphans();

	int Size() const;

	Object* GetObject(unsigned index);

private:
	vector<Object*> _objs;
};

}
