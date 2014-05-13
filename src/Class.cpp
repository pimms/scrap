#include "Class.h"
#include "IndexList.h"
#include "Method.h"


namespace scrap {

Class::Class(int classID, string className)
	:	_super(NULL),	
		_classID(classID),
		_className(className),
		_fieldTemplate(new FieldList()),
		_methods(new MethodList()),
		_stFields(new FieldList()),
		_stMethods(new MethodList()),
		_locked(false)
{
}

Class::~Class()
{
	delete _fieldTemplate;
	delete _methods;
	delete _stMethods;
}


int Class::GetClassID() const 
{
	return _classID;
}

string Class::GetClassName() const
{
	return _className;
}

const Class* Class::GetSuper() const
{
	return _super;
}


void Class::AddFieldTemplate(TypeDesc type)
{
	LockCheck();
	_fieldTemplate->AddField(type);
}

void Class::AddStaticField(TypeDesc type)
{
	LockCheck();
	_stFields->AddField(type);
}

void Class::AddMethod(Method *method)
{
	LockCheck();
	MethodCheck(method, METHOD_NORMAL | METHOD_VIRTUAL);

	_methods->AddMethod(method);
}

void Class::AddStaticMethod(Method *method)
{
	LockCheck();
	MethodCheck(method, METHOD_STATIC);

	_stMethods->AddMethod(method);
}


const Field* Class::GetField(unsigned id) const
{
	return _fieldTemplate->GetField(id);
}

Field* Class::GetStaticField(unsigned id) const 
{
	return _stFields->GetField(id);
}

Method* Class::GetMethod(unsigned id) const
{
	return _methods->GetMethod(id);
}

Method* Class::GetStaticMethod(unsigned id) const
{
	return _stMethods->GetMethod(id);
}


void Class::LockMembers()
{
	if (!_locked) {
		_locked = true;
		_stFields->InstantiateVariables();
	}
}


void Class::SetSuper(const Class *super)
{
	LockCheck();

	if (!super)
		THROW(NullPointerException, "NULL is not a valid superclass");
	if (_super)
		THROW(InvalidOperationException, "Class already has a superclass");

	_super = super;
	_super->LockCheck();

	// Insert members from superclass
	_fieldTemplate->InsertFromSuperclass(_super->_fieldTemplate);
	_methods->InsertFromSuperclass(_super->_methods);
	_stFields->InsertFromSuperclass(_super->_stFields);
	_stMethods->InsertFromSuperclass(_super->_stMethods);
}


void Class::LockCheck() const
{
	if (_locked) {
		THROW(InvalidOperationException,
		"Attempted to perform operation on locked class");
	}
}

void Class::MethodCheck(Method *method, int expectedMap) const
{
	if (!method) {
		THROW(NullPointerException, "Cannot assign NULL method to anything");
	}

	if ((method->GetMethodType() & expectedMap) == 0) {
		THROW(InvalidArgumentException, "Static / normal method mismatch");
	}
}

}

