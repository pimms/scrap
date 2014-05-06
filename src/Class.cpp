#include "Class.h"


namespace scrap {

Class::Class(int classID, string className)
	:	_classID(classID),
		_className(className)
{
	
}

int Class::GetClassID() const 
{
	return _classID;
}

}
