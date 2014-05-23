#include "Program.h"
#include "IndexList.h"
#include "Method.h"
#include "MethodInvocation.h"
#include "Class.h"
#include "Heap.h"

namespace scrap {

Program::Program()
	:	_classList(NULL),
		_mainMethod(NULL),
		_mainClass(NULL),
		_heap(new Heap())
{ }

Program::~Program()
{
	_heap->KillOrphans();
	delete _heap;

	delete _classList;
}


void Program::SetClassList(ClassList *classList)
{
	_classList = classList;
}

ClassList* Program::GetClassList() 
{
	return _classList;
}

const ClassList* Program::GetClassList() const 
{
	return _classList;
}

void Program::SetMainMethod(unsigned classID, unsigned stMethodID)
{
	if (!_classList) {
		THROW(InvalidOperationException,
		"Cannot set the main method before the ClassList has been set");
	}

	_mainClass = _classList->GetClass(classID);
	_mainMethod = _mainClass->GetStaticMethod(stMethodID);
}


void Program::Execute()
{
	MethodInvocation main(_heap, _mainMethod, _mainClass, NULL);
	main.Execute();
}

}
