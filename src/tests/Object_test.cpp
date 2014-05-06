#include "gtest/gtest.h"
#include "../Object.h"
#include "../Heap.h"
#include "../Class.h"

using scrap::Object;
using scrap::Heap;
using scrap::Class;


TEST (ObjectTest, TestRetainCount)
{
	Class c(0, "TestClass");
	Heap heap;
	Object *obj;
	int expected = 1;
	
	ASSERT_NO_THROW(obj = heap.CreateObject(&c));
	
	for (; expected < 100; expected++) {
		ASSERT_EQ(expected, obj->RetainCount());
		obj->Retain();
	}

	while (expected) {
		expected--;
		obj->Release();
		ASSERT_EQ(expected, obj->RetainCount());
	}

	heap.KillOrphans();
}

TEST (ObjectTest, TestTooManyRelease)
{
	// When releasing too much, the reference counter should
	// stay at 0.
	Class c(0, "TestClass");
	Heap heap;
	Object *obj = heap.CreateObject(&c);

	for (int i=0; i<5; i++) {
		obj->Release();
		ASSERT_EQ(obj->RetainCount(), 0);
	}
}

TEST (ObjectTest, TestClassAssignment)
{
	Class c1(1, "Class1");
	Class c2(2, "Class2");

	Heap heap;
	Object *obj1 = heap.CreateObject(&c1);
	Object *obj2 = heap.CreateObject(&c2);

	ASSERT_EQ(obj1->GetClass(), &c1);
	ASSERT_EQ(obj2->GetClass(), &c2);

	obj1->Release();
	obj2->Release();
}

