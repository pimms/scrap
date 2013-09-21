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

	if (Convert(val)) {
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


void Var::operator++() {
	if (mType == INT) {
		mValInt++;
	} else if (mType == FLOAT) {
		mValFloat += 1.f;
	}
}

Var* Var::operator+(const Var &var) const {
	Var *result = new Var();

	// Left associative type
	if (mType == INT) {
		result->Set(mValInt + var.GetInt());
	} else if (mType == FLOAT) {
		result->Set(mValFloat + var.GetFloat());
	}

	return result;
}

void Var::operator+=(const Var &var) {
	if (mType == INT) {
		mValInt += var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat += var.GetFloat();
	}
}


void Var::operator--() {
	if (mType == INT) {
		mValInt--;
	} else if (mType == FLOAT) {
		mValFloat--;
	}
}

Var* Var::operator-(const Var &var) const {
	Var *result = new Var();

	// Left associative type
	if (mType == INT) {
		result->Set(mValInt - var.GetInt());
	} else if (mType == FLOAT) {
		result->Set(mValFloat - var.GetFloat());
	}

	return result;
}

void Var::operator-=(const Var &var) {
	if (mType == INT) {
		mValInt -= var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat -= var.GetFloat();
	}
}


Var* Var::operator*(const Var &var) const {
	Var *result = new Var();

	if (mType == INT) {
		result->Set(mValInt * var.GetInt());
	} else if (mType == FLOAT) {
		result->Set(mValFloat * var.GetFloat());
	}

	return result;
}

void Var::operator*=(const Var &var) {
	if (mType == INT) {
		mValInt *= var.GetInt();
	} else if (mType == FLOAT) {
		mValFloat *= var.GetFloat();
	}
}


Var* Var::operator/(const Var &var) const {
	if (var.GetFloat() == 0.f || !var.GetInt()) {
		throw "DIVISION BY ZERO";
		return NULL;
	}

	Var *result = new Var();

	if (mType == INT) {
		result->Set(mValInt / var.GetInt());
	} else if (mType == FLOAT) {
		result->Set(mValFloat / var.GetFloat());
	}

	return result;
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


Var* Var::operator%(const Var &var) const {
	if (var.GetFloat() == 0.f || !var.GetInt()) {
		throw "MODULO BY ZERO";
		return NULL;
	}

	Var *result = NULL;
	result->Set(GetInt() % var.GetInt());

	return result;
}

void Var::operator%=(const Var &var) {
	Set(GetInt() % var.GetInt());
}


bool Var::operator>(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == GREATER;
}

bool Var::operator>=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return (res == GREATER || res == EQUAL);
}


bool Var::operator<(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == LESS;
}

bool Var::operator<=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return (res == LESS || res == EQUAL);
}


bool Var::operator==(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res == EQUAL;
}

bool Var::operator!=(const Var &var) const {
	CmpResult res = CompareWith(var);
	return res != EQUAL;
}


bool Var::Convert(const char *str) {
	bool dbl = false;

	for (int i=0; i<strlen(str); i++) {
		if (str[i] < '0' && str[i] > '9') {
			// ..all is well
		} else if (str[i] == '.' && !dbl) {
			dbl = true;
		} else if (!i && str[i] != '-') {
			return false;
		}
	}

	if (dbl) {
		Set((float)strtod(str, NULL));
	} else {
		Set(atoi(str));
	}

	return true;
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
		return CompareWithFloat(var);
	} else if (mType == INT && type == INT) {
		return CompareWithInt(var);
	} else if (mType == STRING || type == STRING) {
		return CompareWithString(var);
	}

	return Var::CmpResult::UNDEFCMP;
}

Var::CmpResult Var::CompareWithInt(const Var &iVar) const {
	int tVal = GetInt();
	int oVal = iVar.GetInt();

	if (tVal < oVal) {
		return LESS;
	} else if (tVal > oVal) {
		return GREATER;
	} else if (tVal == oVal) {
		return EQUAL;
	}

	return UNDEFCMP;
}

Var::CmpResult Var::CompareWithFloat(const Var &fVar) const {
	float tVal = GetFloat();
	float oVal = fVar.GetFloat();

	if (abs(tVal-oVal) < 0.0000001f) {
		return EQUAL;
	} if (tVal < oVal) {
		return LESS;
	} else if (tVal > oVal) {
		return GREATER;
	}

	return UNDEFCMP;
}

Var::CmpResult Var::CompareWithString(const Var &sVar) const {
	const char *tVal = GetString();
	const char *oVal = sVar.GetString();

	if (strcmp(tVal, oVal) == 0) {
		return EQUAL;
	} 

	return NEQUAL;
}