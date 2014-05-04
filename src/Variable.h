#pragma once

#include "Scrap.h"

namespace scrap {

class Object;

enum VarType {
	a 		= 0x00,
	OBJECT 	= 0x00,		

	i 		= 0x01,
	INT 	= 0x01,

	f 		= 0x02,
	FLOAT 	= 0x02,

	d 		= 0x04,
	DOUBLE 	= 0x04,

	l 		= 0x08,
	LONG 	= 0x08,

	c 		= 0x10,
	CHAR 	= 0x10,

	b 		= 0x20,
	BOOL 	= 0x20,
};

string VarTypeToString(VarType t);

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

	// All value-getter methods throws InvalidType-exception if
	// their type does not match the getter method called.
	Object* Value_a() const;
	int Value_i() const;
	float Value_f() const;
	double Value_d() const;
	long Value_l() const;
	char Value_c() const;
	bool Value_b() const;

	VarType Type() const;

private:
	VarValue _value;
	VarType _type;

};

}
