#pragma once

#include "Scrap.h"

namespace scrap {

/* Function Body
 * The bytecode itself is contained in the "code" array, and the
 * length of the bytecode array is stored in "length".
 */
struct FunctionBody {
	byte *code;
	unsigned length;
};

class FunctionAttributes {
public:
	FunctionAttributes(TypeDesc ret, vector<TypeDesc> args, string name);

	TypeDesc GetReturnType() const;
	vector<TypeDesc> GetArguments() const;
	string GetName() const;

private:
	vector<TypeDesc> _args;
	TypeDesc _rettype;
	string _name;
};


/* Function 
 * Contains the required definitions for executing a method, but is
 * not itself resopnsible for exetution (see FunctionInvocation).
 */
class Function {
public:
	// The "code"-array of "body" is copied into a new buffer, and the caller is 
	// responsible for disposing of the passed method body-array.
	Function(FunctionBody *body, FunctionAttributes attr);
	~Function();

	const FunctionBody* GetFunctionBody() const;
	const FunctionAttributes& GetFunctionAttributes() const;
	const TypeDesc GetReturnType() const;
	
private:
	FunctionBody _body;
	FunctionAttributes _attrs;
};

}
