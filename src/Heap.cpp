#include "Heap.h"
#include "Object.h"

namespace scrap {

Heap::Heap()
{

}

Heap::~Heap()
{
	KillOrphans();

	if (Size() != 0) {
		THROW(InternalInconsistencyException,
			"Referenced orphans exists at death of heap");
	}
}


Object* Heap::CreateObject(const Class *c)
{
	if (!c) {
		THROW(NullPointerException,
			"Unable to instantiate object with Class *c=NULL")
	}

	Object *obj = new Object(c);
	_objs.push_back(obj);
	return obj;
}


void Heap::KillOrphans()
{
	// TODO
	// Optimize this 
	for (int i=0; i<_objs.size(); i++) {
		if (_objs[i]->RetainCount() == 0) {
			Object *obj = _objs[i];
			delete obj;
			_objs.erase(_objs.begin() + i--);
		}
	}
}


int Heap::Size() const
{
	return _objs.size();
}


}
