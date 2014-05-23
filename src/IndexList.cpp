#include "IndexList.h"
#include "Variable.h"
#include "Method.h"
#include "Class.h"


namespace scrap {

FieldList::FieldList()
	:	_instantiated(false),
		_inheritCount(0)
{ }

FieldList::~FieldList()
{
	if (_instantiated) 
		DeleteVariables();
}

void FieldList::AddField(TypeDesc desc)
{
	if (!_instantiated) {
		_fields.push_back(Field{desc, NULL});
	} else {
		THROW(InvalidOperationException,
			"Unable to add field to instantiated FieldList");
	}
}

Field* FieldList::GetField(unsigned id) 
{
	if (id >= _fields.size())
		THROW(IndexOutOfRangeException,
		"Attempted to access field with index out of range");
	return &(_fields[id]);
}

void FieldList::InstantiateVariables()
{
	if (!_instantiated) {
		_instantiated = true;

		for (int i=0; i<_fields.size(); i++) {
			_fields[i].variable = new Variable(_fields[i].typeDesc.type);
			_fields[i].variable->SetFieldVariableFlag(true);
		}
	}
}

void FieldList::DeleteVariables()
{
	if (_instantiated) {
		_instantiated = false;

		for (int i=_inheritCount; i<_fields.size(); i++) {
			delete _fields[i].variable;
			_fields[i].variable = NULL;
		}
	}
}

void FieldList::InsertFromSuperclass(const FieldList *fieldList) 
{
	if (_fields.size() != 0) 
		THROW(InvalidOperationException,
		"Superclass insertion must occur before any subclass additions");

	_inheritCount = fieldList->_fields.size();
	for (int i=0; i<fieldList->_fields.size(); i++) {
		_fields.push_back(fieldList->_fields[i]);
	}
}



MethodList::MethodList()
	:	_inheritCount(0)
{

}

MethodList::~MethodList()
{
	for (int i=_inheritCount; i<_methods.size(); i++) {
		delete _methods[i];
	}
}

void MethodList::AddMethod(Method *method)
{
	_methods.push_back(method);
}

Method* MethodList::GetMethod(unsigned id) const 
{
	if (id >= _methods.size())
		THROW(IndexOutOfRangeException,
		"Attempted to access field with index out of range");
	return _methods[id];
}

void MethodList::InsertFromSuperclass(const MethodList *methodList)
{
	if (_methods.size() != 0) 
		THROW(InvalidOperationException,
		"Superclass insertion must occur before any subclass additions");

	_inheritCount = methodList->_methods.size();
	for (int i=0; i<methodList->_methods.size(); i++) {
		_methods.push_back(methodList->_methods[i]);
	}
}


ClassList::~ClassList()
{
	for (int i=0; i<_classes.size(); i++) {
		delete _classes[i];
	}
}

void ClassList::AddClass(Class *c, unsigned id)
{
	if (_classes.count(id) != 0) {
		THROW(InvalidOperationException,
			"Attempted to add multiple classes with the same ID");
	}

	_classes[id] = c;
}

Class* ClassList::GetClass(unsigned id) 
{
	if (_classes.count(id) == 0) {
		THROW(NullPointerException,
		"Attempted to get class from undefined ID");
	}
	
	return _classes[id];
}

const Class* ClassList::GetClass(unsigned id) const
{
	if (_classes.count(id) == 0) {
		THROW(NullPointerException,
		"Attempted to get class from undefined ID");
	}
	
	return _classes.at(id);
}

unsigned ClassList::GetClassCount() const 
{
	return _classes.size();
}

}
