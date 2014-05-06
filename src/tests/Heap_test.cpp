#include "gtest/gtest.h"
#include "../Heap.h"
#include "../Object.h"
#include "../Class.h"

using scrap::Object;
using scrap::Heap;
using scrap::Class;


TEST (HeapTest, TestGarbageCollection)
{
	const int totalObj = 10;
	const int released = 5;

	vector<Object*> objs;

	Heap heap;
	Class c(0, "TestClass");

	// Create 10 objects, release half of them
	for (int i=0; i<totalObj; i++) {
		Object *obj = heap.CreateObject(&c);
		if (i < released) {
			obj->Release();
		} else {
			objs.push_back(obj);
		}
	}

	heap.KillOrphans();
	ASSERT_EQ(heap.Size(), totalObj - released);

	while (objs.size()) {
		objs[0]->Release();
		objs.erase(objs.begin());

		heap.KillOrphans();
		ASSERT_EQ(heap.Size(), objs.size());
	}
}


