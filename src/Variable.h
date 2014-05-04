#pragma once

#include "Scrap.h"

namespace scrap {

class Object;


struct VarValue {
	static bool CastAvailable(VarType from, VarType to);

	// Returns a casted VarValue if the cast is legal. 
	// InvalidCastException is thrown if it is illegal.
	static VarValue CastTo(VarValue val, VarType from, VarType to);

	union {
		Object *a;
		int i;
		float f;
		double d;
		long l;
		char c;
		bool b;
	};
};


class Variable {
public:
	Variable(VarType type);
	Variable();
	~Variable();

	// Attempt to cast the current value to the specified type.
	bool Cast(VarType type);

	void Set(Object *a);
	void Set(int i);
	void Set(float f);
	void Set(double d);
	void Set(long l);
	void Set(char c);
	void Set(bool c);

	// All value-getter methods throws InvalidTypeException if
	// their type does not match the getter method called.
	Object* Value_a() const;
	int Value_i() const;
	float Value_f() const;
	double Value_d() const;
	long Value_l() const;
	char Value_c() const;
	bool Value_b() const;

	VarType Type() const;

	// All arithmetic methods require that both Variable objects has
	// the same type. Not all operations are available for all types.
	// InvalidOperationException is thrown is an invalid operation
	// is attempted.
	//
	// All operations act on the called object, as this works fine with
	// how the stack operates during arithmetic operations:
	// 		push A		[A]
	// 		push B		[A,B]
	// 		add			[A]		(A is now equal to A+B)
	void Add(const Variable &var);
	void Sub(const Variable &var);
	void Mul(const Variable &var);
	void Div(const Variable &var);
	void Shr(const Variable &var);
	void Shl(const Variable &var);
	void Mod(const Variable &var);
	void Xor(const Variable &var);
	void And(const Variable &var);
	void Or(const Variable &var);

private:
	VarValue _value;
	VarType _type;

	// Throws either an InvalidOperationException 
	// InvalidTypeException if the operation cannot be performed.
	void ValidOperationCheck(AritOp op, const Variable &var);

	static bool IsOperationAvailable(AritOp op, VarType type);
};

}
