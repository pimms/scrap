#pragma once

#include "Scrap.h"


namespace scrap {

class Variable;
class Method;


struct Field {
	const TypeDesc typeDesc;
	Variable *variable;
};

/* Member Lists
 * Fields and methods are defined within a class using context-unique IDs.
 *
 * In the case of subclasses, the superclass members must be added anew
 * in the subclass list.
 *
 * FieldList and MethodList are currently completely unrelated, but share
 * some common traits of functionality. Consider merging the shared functionality
 * into a superclass if more shared functionality is added.
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
	void AddMethod(Method *method);
	const Method* GetMethod(unsigned id) const;

	// The method list must be empty at the point of insertion
	void InsertFromSuperclass(const MethodList *methodList);

private:
	vector<Method*> _methods;
};

}
