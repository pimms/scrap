#pragma once

#include "Scrap.h"

namespace scrap {

class Class;
class Object;


/* Method Body
 * The bytecode itself is contained in the "code" array, and the
 * length of the bytecode array is stored in "length".
 */
struct MethodBody {
	byte *code;
	unsigned length;
};


/* Method Type
 */
enum MethodType {
	METHOD_NORMAL,
	METHOD_VIRTUAL,
	METHOD_STATIC,
};


class MethodAttributes {
public:
	MethodAttributes(TypeDesc ret, vector<TypeDesc> args);
	MethodAttributes(TypeDesc ret, int argc, ...);

	TypeDesc GetReturnType() const;
	vector<TypeDesc> GetArguments() const;

private:
	vector<TypeDesc> _args;
	TypeDesc _rettype;
};


/* Method
 * Contains the required definitions for executing a method, but is
 * not itself resopnsible for exetution (see MethodInvocation).
 */
class Method {
public:
	// The "code"-array of "body" is copied into a new buffer, and the caller is 
	// responsible for disposing of the passed method body-array.
	Method(MethodType type, Class *c, MethodBody *body, MethodAttributes attr);
	~Method();

	const MethodBody* GetMethodBody() const;
	const Class* GetClass() const;
	MethodType GetMethodType() const;
	const MethodAttributes& GetMethodAttributes() const;
	const TypeDesc GetReturnType() const;
	
private:
	const MethodType _type;
	const Class *_class;
	MethodBody _body;
	MethodAttributes _attrs;
};

}
