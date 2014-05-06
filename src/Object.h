#pragma once

#include "Scrap.h"

namespace scrap {

class Heap;
class Class;

/* Object
 * Class representing an object created with the "new" operator.
 * Objects are reference counted. 
 *
 * Objects must be created via a Heap-instance.
 */
class Object {
public:
	// Increment the reference count by 1. 
	// ONLY CALL THIS ONCE PER ACQUISITION!!!
	void Retain();

	// Decrement retain counter. If the counter reaches 0 as a result of this
	// action, it will be deleted some time in the future. After calling Release(),
	// the caller should invalidate ALL references to this object. 
	void Release();

	unsigned RetainCount() const;

	const Class* GetClass() const;
	
private:
	// Heap is a friend of Object for exclusive creation and deletion
	friend class Heap;

	Object(const Class *c);
	Object(const Object &o) {}
	~Object();

	// The current number of active references on the object.
	unsigned _retains;

	// The class this object belongs to
	const Class *_class;
};

}
