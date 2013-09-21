#pragma once

class Var {
public:
	enum Type {
		UNDEFINED,
		INT,
		FLOAT,
		STRING,
		OBJECT,
	};

	static Var* CreateVar(const char *val);

				Var(int id = 0);
				~Var();
	Type 		GetType() const;
	int 		GetId() const;

	void 		Set(int);
	void 		Set(float);
	void 		Set(const char*);

	int 		GetInt() const;
	float 		GetFloat() const;
	const char*	GetString() const;

	void operator=(const Var &var);

	void operator+=(const Var &var);
	void operator+=(const int &val);
	void operator+=(const float &val);

	void operator-=(const Var &var);
	void operator-=(const int &val);
	void operator-=(const float &val);

	void operator*=(const Var &var);
	void operator*=(const int &val);
	void operator*=(const float &val);

	void operator/=(const Var &var);
	void operator/=(const int &val);
	void operator/=(const float &val);

	void operator%=(const Var &var);
	void operator%=(const int &val);

	bool operator>(const Var &var) const;
	bool operator>=(const Var &var) const;
	
	bool operator<(const Var &var) const;
	bool operator<=(const Var &var) const;

	bool operator==(const Var &var) const;
	bool operator!=(const Var &var) const;

private:
	Type 		mType;
	int 		mValInt;
	float 		mValFloat;
	char* 		mValString;

	int 		mId;

	bool 		Convert(const char *str);
	void 		Clear();


	/***** CmpResult *****
	* When comparing two Vars, a CmpResult
	* defined the state between the two 
	* Vars.
	*
	* The results define the relationship between
	* THIS and OTHER, from the perspective of THIS.
	*
	* Var x = 3
	* Var y = 5
	* x.CompareWith(y) == LESS
	* y.CompareWith(x) == GREATER
	*****/
	enum CmpResult {
		UNDEFCMP	= 0x01,
		GREATER		= 0x02,
		LESS		= 0x04,
		EQUAL		= 0x08,
		NEQUAL		= 0x10,
	};

	CmpResult CompareWith(const Var &var) const;
	CmpResult CompareWithInt(const Var &iVar) const;
	CmpResult CompareWithFloat(const Var &fVar) const;
	CmpResult CompareWithString(const Var &sVar) const;
};