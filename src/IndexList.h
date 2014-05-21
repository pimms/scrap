#pragma once

#include "Scrap.h"


namespace scrap {

class Variable;
class Method;
class Class;


struct Field {
	const TypeDesc typeDesc;
	Variable *variable;
};

/* Index Lists
 * Fields and methods are defined within a class using context-unique IDs.
 *
 *		FieldList and MethodList
 *		In the case of subclasses, the superclass members must be added anew
 * 		in the subclass list.
 *		FieldList and MethodList are currently completely unrelated, but share
 *		some common traits of functionality. Consider merging the shared functionality
 *		into a superclass if more shared functionality is added.
 */


/* Field List
 * A "template" field list is stored in each Class, and copied to
 * instances upon creation. The variables are then instantiated within
 * the instances using the "InstantiateVariables()" method.
 *
 */
class FieldList {
public:
	FieldList();
	~FieldList();

	void AddField(TypeDesc desc);
	Field* GetField(unsigned id);

	void InstantiateVariables();
	void DeleteVariables();

	// The member list must be empty at the point of insertion
	void InsertFromSuperclass(const FieldList *fieldList);

private:
	vector<Field> _fields;
	bool _instantiated;
};


/* Method List
 * Generic method container used for both normal and static methods.
 */
class MethodList {
public:
	~MethodList();

	void AddMethod(Method *method);
	Method* GetMethod(unsigned id) const;

	// The method list must be empty at the point of insertion
	void InsertFromSuperclass(const MethodList *methodList);

private:
	vector<Method*> _methods;
};


/* Class List
 * Contains - believe it or not - a list of classes.
 */
class ClassList {
public:
	~ClassList();

	void AddClass(Class *c, unsigned id);
	Class* GetClass(unsigned id);
	const Class* GetClass(unsigned id) const;
	unsigned GetClassCount() const;

private:
	map<unsigned, Class*> _classes;

};

}
