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

	void operator++();
	Var* operator+(const Var &var) const;
	void operator+=(const Var &var);

	void operator--();
	Var* operator-(const Var &var) const;
	void operator-=(const Var &var);

	Var* operator*(const Var &var) const;
	void operator*=(const Var &var);

	Var* operator/(const Var &var) const;
	void operator/=(const Var &var);

	Var* operator%(const Var &var) const;
	void operator%=(const Var &var);

private:
	Type 		mType;
	int 		mValInt;
	float 		mValFloat;
	char* 		mValString;

	int 		mId;

	bool 		Convert(const char *str);
	void 		Clear();
};