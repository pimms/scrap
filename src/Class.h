#pragma once

#include "Scrap.h"

namespace scrap {

class FieldList;
class MethodList;
class Method;
class Field;

/* Class
 */
class Class {
public:
	Class(int classID, string className);
	~Class();

	int GetClassID() const;
	string GetClassName() const;
	const Class* GetSuper() const;

	void AddFieldTemplate(TypeDesc type);
	void AddStaticField(TypeDesc type);
	void AddMethod(Method *method);
	void AddStaticMethod(Method *method);

	// Instantiates static variables and prevents more additions. This method
	// must be called after the super and ALL members has been set. All classes
	// should be locked simultaneously, as the super cannot be locked when
	// assigning it as super to a sub.
	void LockMembers();

	const Field* GetField(unsigned id) const;
	Field* GetStaticField(unsigned id) const;
	Method* GetMethod(unsigned id) const;
	Method* GetStaticMethod(unsigned id) const;

	void SetSuper(const Class *super);

private:
	const Class *_super;
	string _className;
	int _classID;

	FieldList *_fieldTemplate;
	MethodList *_methods;

	FieldList *_stFields;
	MethodList *_stMethods;

	bool _locked;

	void LockCheck() const;
	void MethodCheck(Method *method, int expectedMap) const;
};

}
