#include "MemberList.h"
#include "Variable.h"
#include "Method.h"

namespace scrap {

FieldList::FieldList()
	:	_instantiated(false)
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
		}
	}
}

void FieldList::DeleteVariables()
{
	if (_instantiated) {
		_instantiated = false;

		for (int i=0; i<_fields.size(); i++) {
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

	for (int i=0; i<fieldList->_fields.size(); i++) {
		_fields.push_back(fieldList->_fields[i]);
	}
}



void MethodList::AddMethod(Method *method)
{
	_methods.push_back(method);
}

const Method* MethodList::GetMethod(unsigned id) const 
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

	for (int i=0; i<methodList->_methods.size(); i++) {
		_methods.push_back(methodList->_methods[i]);
	}
}

}
