#include "Object.h"


namespace scrap {



Object::Object(const Class *c)
	: 	_class(c),
		_retains(0)
{
	// Avoid spawnkilling, lol
	Retain();
}

Object::~Object()
{

}



void Object::Retain()
{
	_retains++;
}

void Object::Release()
{
	if (_retains > 0) 
		_retains--;
}

unsigned Object::RetainCount() const
{
	return _retains;
}


const Class* Object::GetClass() const
{
	return _class;
}

}
