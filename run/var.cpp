#include "var.h"

#include <cstring>
#include <stdlib.h>
#include <assert.h>
#include <sstream>



Var* Var::CreateVar(const char *val) {
	assert(val != NULL && strlen(val) != 0);

	Var *var = new Var();
	var->Set(val);
	return var;
}

Var::Type Var::GetType(string str) {
	bool flt = false;

	for (int i=0; i<str.length(); i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			// ..all is well
		} else if (str[i] == '.') {
			if (flt) {
				return Type::STRING;
			} 

			flt = true;
		} else if (i && str[i] == '-') {
			return Type::STRING;
		}  else {
			return Type::STRING;
		}
	}

	if (flt) {
		return Type::FLOAT;
	} else {
		return Type::INT;
	}
}



Var::Var(int id) {
	mType = UNDEFINED;

	mValInt = 0;
	mValFloat = 0.0f;
	mValString = NULL;

	mId = id;
}

Var::~Var() {
	if (mValString) {
		delete[] mValString;
	}	
}

Var::Type Var::GetType() const {
	return mType;
}

int Var::GetId() const {
	return mId;
}



void Var::Set(int val) {
	Clear();

	mType = INT;
	mValInt = val;
}

void Var::Set(float val) {
	Clear();

	mType = FLOAT;
	mValFloat = val;
}

void Var::Set(const char *val) {
	Clear();

	Var::Type t = GetType(val);
	if (t == INT) {
		Set(atoi(val));
		return;
	} else if (t == FLOAT) {
		Set((float)atof(val));
		return;
	}

	mType = STRING;

	if (mValString != NULL) {
		delete[] mValString;
	}

	mValString = new char[strlen(val)];
	strcpy(mValString, val);
}



int Var::GetInt() const {
	if (mType == INT) {
		return mValInt;
	} else if (mType == FLOAT) {
		return mValFloat;
	} else if (mType == STRING && mValString) {
		return atoi(mValString);
	}
	return 0;
}

float Var::GetFloat() const {
	if (mType == FLOAT) {
		return mValFloat;
	} else if (mType == INT) {
		return mValInt;
	} else if (mType == STRING && mValString) {
		return strtod(mValString, NULL);
	}

	return 0.0;
}

const char* Var::GetString() const {
	if (mType == STRING) {
		return mValString;
	} else if (mType == FLOAT) {
		std::stringstream ss;
		ss << mValFloat;
		return ss.str().c_str();
	} else if (mType == INT) {
		std::stringstream ss;
		ss << mValInt;
		return ss.str().c_str();
	}

	return "NULL";
}



void Var::operator=(const Var &var) {
	Var::Type type = var.GetType();

	if (type == INT) {
		Set(var.GetInt());
	} else if (type == FLOAT) {
		Set(var.GetFloat());
	} else if (type == STRING) {
		Set(var.GetString());
	} else {
		Clear();
	}
}


void Var::operator+=(const Var &var) {
	if (mType == INT) {
		mValInt += var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat += var.GetFloat();
	}
}

void Var::operator+=(const int &val) {
	if (mType == INT) {
		mValInt += val;
	} else if (mType == FLOAT) {
		mValFloat += val;
	} else if (mType == UNDEFINED) {
		Set(val);
	}
}

void Var::operator+=(const float &val) {
	if (mType == FLOAT) {
		mValFloat += val;
	} else if (mType == INT) {
		Set(val + (float)mValInt);
	} else if (mType == UNDEFINED) {
		Set(val);
	}
}



void Var::operator-=(const Var &var) {
	if (mType == INT) {
		mValInt -= var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat -= var.GetFloat();
	}
}

void Var::operator-=(const int &val) {
	if (mType == INT) {
		mValInt -= val;
	} else if (mType == FLOAT) {
		mValFloat -= val;
	} else if (mType == UNDEFINED) {
		Set(-val);
	}
}

void Var::operator-=(const float &val) {
	if (mType == FLOAT) {
		mValFloat -= val;
	} else if (mType == INT) {
		Set((float)mValInt - val);
	} else if (mType == UNDEFINED) {
		Set(-val);
	}
}


void Var::operator*=(const Var &var) {
	if (mType == INT) {
		mValInt *= var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat *= var.GetFloat();
	}
}

void Var::operator*=(const int &val) {
	if (mType == INT) {
		mValInt *= val;
	} else if (mType == FLOAT) {
		mValFloat *= (float)val;
	} 
}

void Var::operator*=(const float &val) {
	if (mType == FLOAT) {
		mValFloat *= val;
	} else if (mType == INT) {
		Set((float)mValInt * val);
	}
}


void Var::operator/=(const Var &var) {
	if (var.GetFloat() == 0.f || !var.GetInt()) {
		throw "DIVISION BY ZERO";
	}

	if (mType == INT) {
		mValInt /= var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat /= var.GetFloat();
	}
}

void Var::operator/=(const int &val) {
	if (mType == INT) {
		mValInt /= val;
	} else if (mType == FLOAT) {
		mValFloat /= (float)val;
	}
}

void Var::operator/=(const float &val) {
	if (mType == FLOAT) {
		mValFloat /= val;
	} else if (mType == INT) {
		Set((float)mValInt / val);
	}
}


void Var::operator%=(const Var &var) {
	Set(GetInt() % var.GetInt());
}

void Var::operator%=(const int &val) {
	if (mType == INT) {
		mValInt %= val;
	}
}


bool Var::operator>(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == GREATER;
}

bool Var::operator>(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return res == GREATER;
}

bool Var::operator>(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return res == GREATER;
}


bool Var::operator>=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return (res == GREATER || res == EQUAL);
}

bool Var::operator>=(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return (res == GREATER || res == EQUAL);
}

bool Var::operator>=(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return (res == GREATER || res == EQUAL);
}


bool Var::operator<(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == LESS;
}

bool Var::operator<(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return res == LESS;
}

bool Var::operator<(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return res == LESS;
}


bool Var::operator<=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return (res == LESS || res == EQUAL);
}

bool Var::operator<=(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return (res == LESS || res == EQUAL);
}

bool Var::operator<=(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return (res == LESS || res == EQUAL);
}


bool Var::operator==(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == EQUAL;
}

bool Var::operator==(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return res == EQUAL;
}

bool Var::operator==(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return res == EQUAL;
}


bool Var::operator!=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res != EQUAL;
}

bool Var::operator!=(const int &val) const {
	CmpResult res = CompareWithInt(val);
	return res != EQUAL;
}

bool Var::operator!=(const float &val) const {
	CmpResult res = CompareWithFloat(val);
	return res != EQUAL;
}


void Var::Clear() {
	mType = UNDEFINED;

	if (mValString) {
		delete[] mValString;
	}

	mValInt = 0;
	mValFloat = 0.0;
}



Var::CmpResult Var::CompareWith(const Var &var) const {
	Var::Type type = var.GetType();

	// If either object is float, the objects are
	// compared as floats. If neither is float and either
	// object is string, the objects are compared as string.
	// Int comparison is only performed when both objects are
	// of type INT.
	if (mType == FLOAT || type == FLOAT) {
		return CompareWithFloat(var.GetFloat());
	} else if (mType == INT && type == INT) {
		return CompareWithInt(var.GetInt());
	} else if (mType == STRING || type == STRING) {
		return CompareWithString(var.GetString());
	}

	return Var::CmpResult::UNDEFCMP;
}

Var::CmpResult Var::CompareWithInt(const int &val) const {
	int tVal = GetInt();

	if (tVal < val) {
		return LESS;
	} else if (tVal > val) {
		return GREATER;
	} else if (tVal == val) {
		return EQUAL;
	}

	return UNDEFCMP;
}

Var::CmpResult Var::CompareWithFloat(const float &val) const {
	float tVal = GetFloat();

	if (abs(tVal-val) < 0.0000001f) {
		return EQUAL;
	} if (tVal < val) {
		return LESS;
	} else if (tVal > val) {
		return GREATER;
	}

	return UNDEFCMP;
}

Var::CmpResult Var::CompareWithString(const char *val) const {
	const char *tVal = GetString();

	if (strcmp(tVal, val) == 0) {
		return EQUAL;
	} 

	return NEQUAL;
}