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



struct Argument {
	VarType type;
	const char *className;
};
typedef Argument ReturnType;


class MethodAttributes {
public:
	MethodAttributes(ReturnType ret, vector<Argument> args);
	MethodAttributes(ReturnType ret, int args, ...);

	ReturnType GetReturnType() const;
	vector<Argument> GetArguments() const;

private:
	vector<Argument> _args;
	ReturnType _rettype;
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
	const ReturnType GetReturnType() const;
	
private:
	const MethodType _type;
	const Class *_class;
	MethodBody _body;
	MethodAttributes _attrs;
};

}
